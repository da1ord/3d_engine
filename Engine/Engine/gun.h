#ifndef GUN_H_
#define GUN_H_

class Gun : Weapon
{
  int ammo_;
  int mag_ammo_;
  int mag_size_;
  float reload_time_;
};

#endif