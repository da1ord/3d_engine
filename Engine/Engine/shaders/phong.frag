#version 330
#extension GL_EXT_texture_array : enable

//precision mediump float;

// Input from vertex shader
//in vec4 varPosition;		          // Vertex position	|WORLD SPACE|
//smooth in vec3 varNormal; 	      // Normal			|WORLD SPACE|
//smooth in vec2 varTexCoord;	      // Texture coords
//smooth in vec3 varTangent;	      // Tangent
//smooth in vec3 varBinormal;	      // Binormal

in vec4 inPosition;				        // Input vertex position
struct Variables {
  vec4 varPosition;			          // Vertex position	|WORLD SPACE|
  vec3 varNormal;		              // Normal			|WORLD SPACE|
  vec2 varTexCoord;	              // Texture coords
  vec3 varTangent;		            // Tangent
  vec3 varBinormal;	              // Binormal
  vec4 vPosition;	                // Vertex position from lightView
};

in Variables vars_fs;

// Textures
uniform sampler2D tex_diff;	      // Texture
uniform sampler2D tex_norm;       // Bump map
uniform float has_diff_tex;       // Has diffuse texture
uniform float has_norm_tex;       // Has normal texture

// Shadows
uniform sampler2DArray shadowMap; // Shadow map
uniform	mat4 shadowMVP[3];        // Shadow map matrices

in float viewZ_fs;

uniform vec3 farPlanes;
int level = 0;

// Material properties
uniform vec4 ambientColor;	      // Material ambient
uniform vec4 diffuseColor;	      // Material diffuse
uniform vec4 specularColor;	      // Material specular
uniform float shininess;	        // Material shininess
uniform float alpha;		          // Material alpha

// Lighting
uniform	mat4 V_inv;			          // Inverse view matrix
const vec4 light0_pos = vec4(-500.0, 500.0, 500.0, 1.0);
const vec4 light0_col = vec4(1.0);

// Output to frame buffer
out vec4 outColor;	// Final fragment color

const float levelBias = 1.0011;

// Enable/disable rendering of shadows
uniform bool shadows;
// Enable/disable rendering of colored shadow map frustums
uniform bool color_csm;

float rand(vec2 co) {
  return fract(sin(dot(co.xy, vec2(12.9898, 78.233))) * 43758.5453);
}

float sampleShadow(vec2 shadowCoord, sampler2DArray map, float bias, float fragDepth)  {
  float shadowDepth = texture(map, vec3(shadowCoord, level)).r;
  //if (shadowDepth > 0.999) {
  //  return 1.0;
  //}
    float depth = fragDepth / shadowDepth;
    return depth > bias ? 0.0 : 1.0;
}

float pcfFilter(vec3 shadowCoord, sampler2DArray map, float bias) {
  vec2 uv = shadowCoord.xy;
  float fragDepth = shadowCoord.z;

  if (sampleShadow(uv, map, bias, fragDepth) < 1.0) {
        float result = 0.0;
        int cnt = 0;
    float l2 = pow(2.f, level + 1.f);
    vec2 size = vec2(1.0 / 2048.0 / l2, 1.0 / 1200.0 / l2); // 1.0 / texSize
    for (float x = -1.0; x <= 1.0; x++) {
      for (float y = -1.0; y <= 1.0; y++) {
        float jitter = rand(uv) * 2.0f - 1.0f;
        vec2 offset = vec2(x, y) * size * jitter * 1.25;
        uv = clamp(uv + offset, 0.0, 1.0);

        result += sampleShadow(uv, map, bias, fragDepth);
      }
        }
    return result / 9.0;
    }
    else return 1.0;
}

void main(void) {
  // Prepare data from texture files
  vec4 texel = vec4(1, 1, 1, 1);
  if (has_diff_tex > 0) {
    texel = texture(tex_diff, vars_fs.varTexCoord);
  }
  
  vec3 normal = normalize(vars_fs.varNormal);
  //if (has_norm_tex > 0) {
  //  // TBN matrix
  //  mat3 TBN = mat3(normalize(vars_fs.varTangent), 
  //    normalize(vars_fs.varBinormal), normalize(vars_fs.varNormal));
  //  normal = normalize(vars_fs.varNormal + TBN * normalize(texture(tex_norm, 
  //    (vars_fs.varTexCoord + 32.0) / 64.0).rgb * 2.0 - 1.0));
  //}

  // View direction in world space
  vec3 view_dir = normalize(vec3(V_inv * vec4(0.0, 0.0, 0.0, 1.0) - vars_fs.varPosition));
  //view_dir = normalize(vec3(varPosition));

  // Light direction
  vec3 light_dir = normalize(vec3(light0_pos));
  //vec3 light_dir = normalize(vec3(light0_pos - vars_fs.varPosition));

  // Compute Phong lighting components
  vec4 ambient = ambientColor * texel;
  float dim = max(dot(normal, light_dir), 0.0);
  vec4 diffuse = light0_col * diffuseColor * dim * texel;
  vec3 reflection = reflect(-light_dir, normal);
  vec4 specular = light0_col * specularColor * pow(max(dot(reflection, view_dir), 0.0), shininess);

  // Render shadows
  if (shadows) {
	  // Select shadowmap
	  if (viewZ_fs > farPlanes.z) {     // out of view frustum
		level = -1;
	  }
	  else if (viewZ_fs > farPlanes.y) {// farthest frustum
		level = 2;
	  }
	  else if (viewZ_fs > farPlanes.x) {// middle frustum
		level = 1;
	  }
	  else {                            // nearest frustum
		level = 0;
	  }

	  // Shadowtest
	  if (has_norm_tex < 1 && level > -1) {
		vec4 shadowUV = shadowMVP[level] * vars_fs.vPosition;
		shadowUV.xyz /= shadowUV.w;
    
		/*if (shadowUV.x <= 1.0 && shadowUV.x >= 0.0 && 
			shadowUV.y <= 1.0 && shadowUV.y >= 0.0) */
		  {
		  float shadow = pcfFilter(shadowUV.xyz, shadowMap, levelBias);
		  if (dim >= 0.0) {
			  diffuse *= shadow;
		  }
		}
	  }
  }
  
  // Render colored shadow map frustums
  if (color_csm) {
    if (level == 0) {     // farthest frustum
      diffuse.r = 1.0f;
      diffuse.g = 0.0f;
      diffuse.b = 0.0f;
    }
    else if (level == 1) {// middle frustum
      diffuse.r = 0.0f;
      diffuse.g = 0.0f;
      diffuse.b = 1.0f;
    }
    else {                // nearest frustum
      diffuse.r = 0.0f;
      diffuse.g = 1.0f;
      diffuse.b = 0.0f;
    }
  }
  
  // Combine final fragment color
  //diffuse = max(diffuse, 0.1); 
  // TODO: specular is bad - black artifacts
  outColor = clamp(ambient + diffuse, 0.0, 1.0);// + specular
  if (has_norm_tex > 0) {
    outColor = clamp(texel, 0.0, 1.0);
  }
  
  outColor.a = alpha;
  
  //outColor2 = vec4(1.0, 0.0, 0.0, 1.0);
  //outColor3 = vec4(0.0, 1.0, 0.0, 1.0);
  //outColor4 = vec4(0.0, 0.0, 1.0, 1.0);
}