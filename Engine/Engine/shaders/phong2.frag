#version 140

precision mediump float;

// Input from vertex shader
in vec4 varPosition;                // Vertex position	|WORLD SPACE|
smooth in vec3 varNormal;           // Normal		|WORLD SPACE|
in vec2 varTexCoord;         // Texture coords
in vec3 varTangent;          // Tangent
in vec3 varBinormal;         // Binormal
in vec4 varLightPos;

// Textures
uniform sampler2D tex_diff;         // Texture
uniform sampler2D tex_norm;         // Bump map
uniform sampler2D shadowMap;        // Shadow map

// Material properties
uniform vec4 ambientColor;          // Material ambient
uniform vec4 diffuseColor;          // Material diffuse
uniform vec4 specularColor;         // Material specular
uniform float shininess;            // Material shininess
uniform float alpha;		    // Material alpha

uniform float hasDiff;
uniform float hasBump;

// Lighting
uniform	mat4 VI;			      // Inverse view matrix
const vec4 lightPos = vec4(-100.0, 400.0, -50.0, 1.0);
const vec4 lightCol = vec4(1.0);

// Output to frame buffer
out vec4 outColor;			    // Final fragment color

float rand(vec2 co) {
  return fract(sin(dot(co.xy, vec2(12.9898, 78.233))) * 43758.5453);
}

float sampleShadow(vec2 shadowCoord, sampler2D map, float bias, float fragDepth)  {
  float shadowDepth = texture(map, shadowCoord).r;
	
  float depth = fragDepth / shadowDepth;
	return depth > bias ? 0.0 : 1.0;
}

float gaussFilter(vec3 shadowCoord, sampler2D map, float bias) {
  vec2 uv = shadowCoord.xy;
  float fragDepth = shadowCoord.z;

	if (sampleShadow(uv, map, bias, fragDepth) < 1.0) {
		float result = 0.0;
		int cnt = 0;
    vec2 size = vec2(0.25 / 1280.0, 0.25 / 640.0); // 1.0 / texSize
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


void main() {
  // TBN matrix
  mat3 TBN = mat3(normalize(varTangent), normalize(varBinormal), normalize(varNormal));

  // Prepare data from texture files
  vec4 texel = vec4(1.0f);
  vec3 normal = varNormal;

  if (hasDiff == 1.0f) {
    texel = texture2D(tex_diff, varTexCoord);
  }

  if (hasBump == 1.0f) {
    normal += TBN * normalize(texture2D(tex_norm, varTexCoord).rgb * 2.0 - 1.0);
  }
  normal = normalize(normal);

  // View direction in world space
  vec3 viewDir = normalize(vec3(VI * vec4(0.0, 0.0, 0.0, 1.0) - varPosition));

  // Light direction
  vec3 lightDir = normalize(vec3(lightPos - varPosition));

  // Compute Phong lighting components
  vec4 ambient = ambientColor * texel;
  outColor = ambient;

  float lambert = max(dot(normal, lightDir), 0.0);

  /* Shadows */
  float shadow = 1.0;
  vec4 shadowUV = varLightPos;
  shadowUV.xyz /= shadowUV.w;

  if (shadowUV.x > 1.0 || shadowUV.x < 0.0 || 
    shadowUV.y > 1.0 || shadowUV.y < 0.0) {
	  shadow = 1.0;
  }
  else {
    shadow = gaussFilter(shadowUV.xyz, shadowMap, 1.00001);
    //shadow = sampleShadow(shadowUV.xy, shadowMap, 1.00001, shadowUV.z);
  }

  if (lambert > 0.0f) {
    vec4 diffuse = lightCol * diffuseColor * lambert * texel * shadow;
    vec3 reflection = reflect(-lightDir, normal);
    vec4 specular = lightCol * specularColor * pow(max(dot(reflection, viewDir), 0.0), shininess);
    
    outColor += diffuse + specular;
    outColor = clamp( outColor, 0.0, 1.0);
  }

  outColor.a = alpha;
}
