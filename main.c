///////////////////////////////

#include <acknex.h>
#include <ackphysx.h>
#include <camera.c>
#include <default.c>

#include "mtlFX.c"
var vg;
var lap;
var test;
var test2;
SOUND *carai1_wav = "engine.ogg";
var FTrafficCount = 0;
///////////////////////////////

var FSelect = 0;

PANEL *my_panel2 = {pos_x = 0;
pos_y = 0;
digits(10, 10, "Use A & D to select a car", "Arial#20bi", 1, vg);
digits(10, 40, "Press Enter to race!", "Arial#20bi", 1, vg);
flags = SHOW;
}

PANEL *my_panel = {pos_x = 0;
pos_y = 0;
digits(10, 10, "Position %2.0f", "Arial#20bi", 1, vg);
digits(10, 30, "Lap %2.0f", "Arial#20bi", 1, lap);
}

PANEL *winner_panel = {
pos_x = 0;
pos_y = 0;
digits(0, 0, "YOU ARE WINNER", "Arial#60bi", 1, vg);

}

PANEL *loser_panel = {pos_x = 0;
pos_y = 0;
digits(0, 0, "YOU ARE LOSER", "Arial#60bi", 1, vg);

}

ENTITY *skycube = {type = "skycube+6.png"; // the image for a backdrop scene
                                           //	type = "clouds+6.tga";
flags2 = SKY | CUBE | VISIBLE;
}

VIEW *fxa_mirror_view =

    {

        layer = 10;
}

ENTITY *FCheck1;
ENTITY *FCheck2;
ENTITY *FCheck3;
ENTITY *FCheck4;
ENTITY *FCheck5;
ENTITY *FCar1;
ENTITY *FCar2;

action Acheckpoint() {
  if (my.skill1 == 1) {
    FCheck1 = me;
  }
  if (my.skill1 == 2) {
    FCheck2 = me;
  }
  if (my.skill1 == 3) {
    FCheck3 = me;
  }
  if (my.skill1 == 4) {
    FCheck4 = me;
  }
  if (my.skill1 == 5) {
    FCheck5 = me;
  }
}

function actor(type) {
  pXent_settype(my, PH_RIGID, type);
  pXent_setelasticity(my, 30);
  pXent_setmass(my, 300);
  pXent_setdamping(my, 10, 30);
}

// function check node skills. skills is a number of next node. if skill0==0, then result is a next node.(node+1, path edge direction is not use)
// if you need reverse direction, you need setup every node skill0
// you need set skils on the cross roads and on end of paths

var GetNextNode(var ANodeNum, var *ASkills) {
  var node_count = 0;
  var result = 0;
  if (ASkills[0] != 0) {
    node_count = 1;
  }
  if (ASkills[1] != 0) {
    node_count = 2;
  }
  if (ASkills[2] != 0) {
    node_count = 3;
  }
  if (ASkills[3] != 0) {
    node_count = 4;
  }
  if (ASkills[4] != 0) {
    node_count = 5;
  }
  if (ASkills[5] != 0) {
    node_count = 6;
  }
  if (node_count > 1) {
    random_seed(0);

    node_count = ASkills[floor(random(node_count))];
    if (node_count == 0) {
      result = ANodeNum + 1;
    } else {
      result = node_count;
    }
  } else {
    if (ASkills[0] == 0) {
      result = ANodeNum + 1;
    } else {
      result = ASkills[0];
    }
  }
  return result;
}

var GetNextNode2(ENTITY *enemy, var ANodeNum, var *ASkills) {
  var node_count = 0;
  var result = 0;
  var ldist1 = 0;
  var ldist2 = 0;
  var ldist3 = 0;
  VECTOR vnode1;
  ldist1 = vec_dist(my.x, player.x);

  path_getnode(enemy, ANodeNum, vnode1, ASkills);
  ldist2 = vec_dist(my.x, vnode1);
  ldist3 = vec_dist(player.x, vnode1);

  if (ASkills[0] != 0) {
    node_count = 1;
  }
  if (ASkills[1] != 0) {
    node_count = 2;
  }
  if (ASkills[2] != 0) {
    node_count = 3;
  }
  if (ASkills[3] != 0) {
    node_count = 4;
  }
  if (ASkills[4] != 0) {
    node_count = 5;
  }
  if (ASkills[5] != 0) {
    node_count = 6;
  }
  if (node_count > 1) {

    random_seed(0);

    node_count = ASkills[floor(random(node_count))];
    if (node_count == 0) {
      result = ANodeNum + 1;
    } else {
      result = node_count;
    }

  } else {
    if (ASkills[0] == 0) {
      result = ANodeNum + 1;
    } else {
      result = ASkills[0];
    }
  }
  return result;
}

action ACar_Enemy() {
  wait(1);
  while (!player) {
    wait(1);
  }
  set(my, FLAG2);
  if (!FCar1) {
    FCar1 = me;
  } else {
    FCar2 = me;
  }
  ANGLE temp3;
  set(my, FLAG1);
  set(my, TRANSLUCENT);
  my.alpha = 0;
  path_set(me, "path_000");
  FTrafficCount += 1;
  var node_num1 = 1;
  var node_num2;
  var node_num3;
  var node_num4;
  var node_count = 1;
  VECTOR tempv;
  VECTOR tempang;
  VECTOR vnode1;
  VECTOR vnode2;
  VECTOR vnode3;
  VECTOR vnode4;
  VECTOR temp;
  var nodeskills[6];
  wait(1);
  var z = 999999;
  var p;
  var v;
  node_num1 = 1;
  my.skill19 = 2;

  // lets find nearest node and set it as first node

  while (path_getnode(my, node_num1, vnode1, nodeskills) != 0) {
    v = vec_dist(my.x, vnode1);
    if (v <= z) {
      z = v;
      node_count = node_num1;
    }
    node_num1 += 1;
  }
  // and then get next nodes
  z = 0;
  node_num1 = node_count;
  path_getnode(my, node_num1, vnode1, nodeskills);

  node_num2 = GetNextNode(node_num1, nodeskills);
  path_getnode(my, node_num2, vnode2, nodeskills);

  node_num3 = GetNextNode(node_num2, nodeskills);
  path_getnode(my, node_num3, vnode3, nodeskills);

  node_num4 = GetNextNode(node_num3, nodeskills);
  path_getnode(my, node_num4, vnode4, nodeskills);

  var accel = 0;
  var steer = 0;
  var gravity[3] = {0, 0, -386};

  actor(PH_CONVEX);
  pXent_setfriction(my, 50);
  // my.material = mtl_DynEnvMap;

  VECTOR temp2;
  pXent_setmassoffset(my, vector(0, 0, -30), NULL);
  pXent_setmass(my, 100);
  ENTITY *FLwheel = ent_create("buggy_rad1.mdl", vector(my.x + 86, my.y - 43, my.z - 20), NULL);
  ENTITY *FRwheel = ent_create("buggy_rad1.mdl", vector(my.x + 86, my.y + 43, my.z - 20), NULL);
  ENTITY *BLwheel = ent_create("buggy_rad1.mdl", vector(my.x - 70, my.y + 43, my.z - 20), NULL);
  ENTITY *BRwheel = ent_create("buggy_rad1.mdl", vector(my.x - 70, my.y - 43, my.z - 20), NULL);
  pXcon_add(PH_WHEEL, FLwheel, me, 0);
  pXcon_add(PH_WHEEL, FRwheel, me, 0);
  pXcon_add(PH_WHEEL, BLwheel, me, 0);
  pXcon_add(PH_WHEEL, BRwheel, me, 0);
  temp.x = 10000;
  temp.y = 500;
  temp.z = 6;

  temp2.x = 500;
  temp2.y = 100;
  temp2.z = 0;
  pXcon_setparams2(FLwheel, NULL, temp2, temp);
  pXcon_setparams2(FRwheel, NULL, temp2, temp);
  pXcon_setparams2(BLwheel, NULL, temp2, temp);
  pXcon_setparams2(BRwheel, NULL, temp2, temp);
  set(FLwheel, INVISIBLE);
  set(FRwheel, INVISIBLE);
  set(BLwheel, INVISIBLE);
  set(BRwheel, INVISIBLE);

  vec_set(temp, vnode2);
  vec_sub(temp, my.x);
  temp3.pan = my.pan;
  vec_to_angle(temp3.pan, temp);
  temp3.roll=0;
  temp3.tilt=0;
  pXent_rotate(my, NULL, temp3);
  pXent_addforcelocal(me, nullvector, _vec(5000, 0, 0));
  STRING *debugs = "1";
  ANGLE vangle;
  ANGLE v3angle;
  var yp;
  var brake;
  wait(1);
  var carai_engine = ent_playloop(my, carai1_wav, 6000);

  while (my.alpha < 100) {
    my.alpha += 1;
    pXent_addforcelocal(me, _vec(100 * cos(my.pan), 100 * sin(my.pan), 0), _vec(100, 0, 0));
    wait(1);
  }
  reset(my, TRANSLUCENT);
  AssignEnvMap(my);
  var lanim, lamin2;
  while (1) {

    //	camera.x=my.x;
    //	camera.y=my.y;

    if (vec_dist(my.x, vnode1) > 500) {
      accel = 3050;
    } else {
      accel = 2050;
      node_num1 = node_num2;
      path_getnode(my, node_num1, vnode1, nodeskills);

      node_num2 = node_num3;
      path_getnode(my, node_num2, vnode2, nodeskills);

      node_num3 = node_num4;
      path_getnode(my, node_num3, vnode3, nodeskills);

      node_num4 = GetNextNode(node_num3, nodeskills);
      path_getnode(my, node_num4, vnode4, nodeskills);
    }
    // calculate bezier curve from my.x, node1 and node2 to get some smooth steering
    vec_set(tempv, my.x);
    vec_add(tempv, vnode2);
    vec_scale(tempv, 0.5);

    vec_add(tempv, vnode1);
    vec_scale(tempv, 0.5);

    // if AI to close to nodes, then get next nodes
    if (vec_dist(my.x, tempv) < 500) {
      node_num1 = node_num2;
      path_getnode(my, node_num1, vnode1, nodeskills);

      node_num2 = node_num3;
      path_getnode(my, node_num2, vnode2, nodeskills);

      node_num3 = node_num4;
      path_getnode(my, node_num3, vnode3, nodeskills);

      node_num4 = GetNextNode(node_num3, nodeskills);
      path_getnode(my, node_num4, vnode4, nodeskills);
    }

    if (vec_dist(my.x, vnode2) < 500) {
      node_num1 = node_num2;
      path_getnode(my, node_num1, vnode1, nodeskills);

      node_num2 = node_num3;
      path_getnode(my, node_num2, vnode2, nodeskills);

      node_num3 = node_num4;
      path_getnode(my, node_num3, vnode3, nodeskills);

      node_num4 = GetNextNode(node_num3, nodeskills);
      path_getnode(my, node_num4, vnode4, nodeskills);
    }

    if (vec_dist(my.x, vnode3) < 500) {
      node_num1 = node_num2;
      path_getnode(my, node_num1, vnode1, nodeskills);

      node_num2 = node_num3;
      path_getnode(my, node_num2, vnode2, nodeskills);

      node_num3 = node_num4;
      path_getnode(my, node_num3, vnode3, nodeskills);

      node_num4 = GetNextNode(node_num3, nodeskills);
      path_getnode(my, node_num4, vnode4, nodeskills);
    }

    //	draw_line3d(tempv,NULL,100); // move to first corner
    //	draw_line3d(my.x,vector(0,255,0),100);
    //	draw_line3d(vnode1,vector(255,0,0),100);
    //	draw_line3d(vnode2,vector(255,0,0),100);
    //	draw_line3d(vnode3,vector(255,0,0),100);
    //	draw_line3d(vnode4,vector(255,0,0),100);

    temp.x = accel * cosv(my.pan);
    temp.y = accel * sinv(my.pan);
    temp.z = 0;

    vec_set(tempang, tempv);
    vec_sub(tempang, my.x);
    vec_to_angle(vangle, tempang);

    vec_set(tempang, vnode4);
    vec_sub(tempang, my.x);
    vec_to_angle(v3angle, tempang);

    steer = 0;

    if (v3angle.pan - my.pan > 180) {
      v3angle.pan = v3angle.pan - 360;
    }
    if (my.pan - v3angle.pan > 180) {
      v3angle.pan = v3angle.pan + 360;
    }

    if (vangle.pan - my.pan > 180) {
      vangle.pan = vangle.pan - 360;
    }
    if (my.pan - vangle.pan > 180) {
      vangle.pan = vangle.pan + 360;
    }

    if (vangle.pan > my.pan) {
      steer = clamp(vangle.pan - my.pan, 0, 20);
    } else {
      steer = clamp(vangle.pan - my.pan, -20, 0);
    }

    if (v3angle.pan > my.pan) {
      brake = v3angle.pan - my.pan;
    } else {
      brake = v3angle.pan - my.pan;
    }

    // if 1st node have big angle, AI slow down
    if (abs(steer) > 20) {
      accel = 2700;
    }
    if (abs(steer) > 25) {
      accel = 2500;
    }
    if (abs(steer) > 30) {
      accel = 1550;
    }

    temp.x = accel * cosv(my.pan);
    temp.y = accel * sinv(my.pan);
    temp.z = 0;

    pXent_getvelocity(my, temp, nullvector);
    v = vec_dist(temp, nullvector) / 10;
    my.skill1 = v;
    snd_tune(carai_engine, 0, v, 0);
    lanim = lanim + v / 300;

    // if 3rd path node have big angle, AI will slow down or stop
    if (abs(brake) > 10) {
      if (v > 30) {
        accel = 3800;
      } else {
        accel = 3000;
      }
      if (v > 60) {
        accel = -800;
      }
    }

    str_cpy(debugs, ""); // clear the string
    z = c_scan(my.x, my.pan, vector(25, 80, 1000), IGNORE_WORLD | IGNORE_ME | SCAN_FLAG2);
    if (you) {
      p = my.pan;
      yp = you.pan;

      if (p < 0) {
        p += 360;
      }
      if (yp < 0) {
        yp += 360;
      }

      if (p > 360) {
        p -= 360;
      }
      if (yp > 360) {
        yp -= 360;
      }

      if ((yp < 30) && (p > 330)) {
        yp += 180;
        p -= 180;
      }

      if ((p < 30) && (yp > 330)) {
        p += 180;
        yp -= 180;
      }

      // if AI saw something with +- same pan...
      if (abs(p - yp) < 30) {
        //	draw_line3d(my.x,NULL,100);
        //	draw_line3d(you.x,vector(0,0,255),100);

        accel = 0;
        if (v > 10) // and AI have velocity >10
        {
          if (z < 800) // and it near than 800 quants, AI must stop
          {
            accel = 1500;
            steer += 30;
            //	str_cat(debugaas,"BRAKE!");
          }
        } else {
          if (z < 500) {
            if (z < 400) {
              if (v >= 10) // if we have velocity <10 and >1, AI will slowdown
              {
                accel = 800;
                str_cat(debugs, "brake");
                steer += 20;
              }
            } else {
              if (v < 10) // if AI stoped to early, AI can add some speed
              {
                accel = 2200;
                str_cat(debugs, "go");
                steer += 10;
              }
            }
          } else {
            if (v < 4) // if AI stoped to early, AI can add some speed
            {
              accel = 2000;
              str_cat(debugs, "go");
              steer += 10;
            }
          }
        }
      } else // if AI seen another car at another angle with velosity > 5, AI will slowdown
      {
        if (is(you, FLAG1)) {
          if (you.skill1 > 5) {
            if (v >= 1) {
              accel = 1800;
              str_cat(debugs, "brake pass");
              steer += 20;
            }
          }
        }
      }

      temp.z += 100;
      vec_set(temp, my.x);
      //	if (vec_to_screen(temp,camera))
      //	draw_text(debugs,temp.x,temp.y,vector(100,100,255));
    }
    // if another car is near on the right side, AI will pass it
    z = c_scan(my.x, my.pan - 90, vector(160, 180, 150), IGNORE_ME | SCAN_FLAG2);
    if (you) {
      temp.z += 100;
      vec_set(temp, my.x);
      if (v > 5) {
        //	accel=-1800;
        steer += 20;
      }
    }

    if (accel < 0) {
      lamin2 = 1;
    } else {
      lamin2 = 0;
    }
    if (lanim > 2) {
      lanim = 0;
    }
    my.skin = 1 + (lamin2 * 2) + lanim;

    if ((my.z < 700) || ((v < 1) && (abs(my.roll) > 80) || (abs(my.tilt) > 80))) {
      pXent_enable(me, 0);
      pXent_enable(FLwheel, 0);
      pXent_enable(FRwheel, 0);
      pXent_enable(BLwheel, 0);
      pXent_enable(BRwheel, 0);

      my.x = vnode1.x;
      my.y = vnode1.y;
      my.z = vnode1.z;
      my.roll = 0;
      my.tilt = 0;
      my.pan = 0;

      FLwheel.roll = 0;
      FLwheel.tilt = 0;
      FLwheel.pan = 0;
      FLwheel.x = my.x + 86;
      FLwheel.y = my.y - 43;
      FLwheel.z = my.z - 20;

      FRwheel.roll = 0;
      FRwheel.tilt = 0;
      FRwheel.pan = 0;
      FRwheel.x = my.x + 86;
      FRwheel.y = my.y + 43;
      FRwheel.z = my.z - 20;

      BLwheel.roll = 0;
      BLwheel.tilt = 0;
      BLwheel.pan = 0;
      BLwheel.x = my.x - 70;
      BLwheel.y = my.y + 43;
      BLwheel.z = my.z - 20;

      BRwheel.roll = 0;
      BRwheel.tilt = 0;
      BRwheel.pan = 0;
      BRwheel.x = my.x - 70;
      BRwheel.y = my.y - 43;
      BRwheel.z = my.z - 20;

      pXent_enable(me, 1);

      pXent_setmassoffset(my, vector(0, 0, -30), NULL);
      pXent_setmass(my, 100);
      pXcon_add(PH_WHEEL, FLwheel, me, 0);
      pXcon_add(PH_WHEEL, FRwheel, me, 0);
      pXcon_add(PH_WHEEL, BLwheel, me, 0);
      pXcon_add(PH_WHEEL, BRwheel, me, 0);
      temp.x = 10000;
      temp.y = 500;
      temp.z = 6;

      temp2.x = 500;
      temp2.y = 100;
      temp2.z = 0;
      pXcon_setparams2(FLwheel, NULL, temp2, temp);
      pXcon_setparams2(FRwheel, NULL, temp2, temp);
      pXcon_setparams2(BLwheel, NULL, temp2, temp);
      pXcon_setparams2(BRwheel, NULL, temp2, temp);

      pXent_enable(FLwheel, 1);
      pXent_enable(FRwheel, 1);
      pXent_enable(BLwheel, 1);
      pXent_enable(BRwheel, 1);
      vec_set(temp, vnode2);
      vec_sub(temp, my.x);
       temp3.pan = my.pan;
      vec_to_angle(temp3.pan, temp);
      temp3.roll=0;
  temp3.tilt=0;
      pXent_rotate(my, NULL, temp3);
      pXent_addforcelocal(me, nullvector, _vec(10000, 0, 0));
      accel=10000;
    }

    if (my.skill19 == 12) {
      accel = 0;
    } else if ((my.skill19 == 11) || (my.skill19 == 1) || (my.skill19 == 6)) {
      if (vec_dist(my.x, FCheck1.x) < 1000) {
        my.skill19 = my.skill19 + 1;
      } else {
        my.skill18 = vec_dist(my.x, FCheck1.x);
      }
    }
    if ((my.skill19 == 2) || (my.skill19 == 7)) {
      if (vec_dist(my.x, FCheck2.x) < 1000) {
        my.skill19 = my.skill19 + 1;
      } else {
        my.skill18 = vec_dist(my.x, FCheck2.x);
      }
    }
    if ((my.skill19 == 3) || (my.skill19 == 8)) {
      if (vec_dist(my.x, FCheck3.x) < 1000) {
        my.skill19 = my.skill19 + 1;
      } else {
        my.skill18 = vec_dist(my.x, FCheck3.x);
      }
    }
    if ((my.skill19 == 4) || (my.skill19 == 9)) {
      if (vec_dist(my.x, FCheck4.x) < 1000) {
        my.skill19 = my.skill19 + 1;
      } else {
        my.skill18 = vec_dist(my.x, FCheck4.x);
      }
    }
    if ((my.skill19 == 5) || (my.skill19 == 10)) {
      if (vec_dist(my.x, FCheck5.x) < 1000) {
        my.skill19 = my.skill19 + 1;
      } else {
        my.skill18 = vec_dist(my.x, FCheck5.x);
      }
    }

    if ((accel > 0) && (v < 2) && (abs(steer) > 10)) {
      vec_set(temp, vnode2);
      vec_sub(temp, my.x);
        temp3.pan = my.pan;
      vec_to_angle(temp3.pan, temp);
      temp3.roll=0;
  temp3.tilt=0;
      pXent_rotate(my, NULL, temp3);
    }

    pXcon_setwheel(FLwheel, -steer, -accel, 0);
    pXcon_setwheel(FRwheel, -steer, -accel, 0); // steer to the right
    pXcon_setwheel(BLwheel, 0, -accel, 0);
    pXcon_setwheel(BRwheel, 0, -accel, 0);
    vec_set(temp, my.x);
    temp.z += 100;
    str_cpy(debugs, "v");
    //	str_cat_num(debugs,"%.0f",accel);
    if (vec_to_screen(temp, camera))
      draw_text(debugs, temp.x, temp.y, vector(100, 100, 255));

    wait(1);
  }
}

action ACar_Traffic() {
  while (!player) {
    wait(1);
  }
  set(my, FLAG2);
  set(my, FLAG1);
  set(my, TRANSLUCENT);
  my.alpha = 0;
  path_set(me, "path_000");
  FTrafficCount += 1;
  var node_num1 = 1;
  var node_num2;
  var node_num3;
  var node_num4;
  var node_count = 1;
  VECTOR tempv;
  VECTOR tempang;
  VECTOR vnode1;
  VECTOR vnode2;
  VECTOR vnode3;
  VECTOR vnode4;
  VECTOR temp;
  var nodeskills[6];
  wait(1);
  var z = 999999;
  var p;
  var v;
  node_num1 = 1;

  // lets find nearest node and set it as first node

  while (path_getnode(my, node_num1, vnode1, nodeskills) != 0) {
    v = vec_dist(my.x, vnode1);
    if (v <= z) {
      z = v;
      node_count = node_num1;
    }
    node_num1 += 1;
  }
  // and then get next nodes
  z = 0;
  node_num1 = node_count;
  path_getnode(my, node_num1, vnode1, nodeskills);

  node_num2 = GetNextNode(node_num1, nodeskills);
  path_getnode(my, node_num2, vnode2, nodeskills);

  node_num3 = GetNextNode(node_num2, nodeskills);
  path_getnode(my, node_num3, vnode3, nodeskills);

  node_num4 = GetNextNode(node_num3, nodeskills);
  path_getnode(my, node_num4, vnode4, nodeskills);

  var accel = 0;
  var steer = 0;
  var gravity[3] = {0, 0, -386};

  actor(PH_CONVEX);
  pXent_setfriction(my, 50);
  // my.material = mtl_DynEnvMap;

  VECTOR temp2;
  pXent_setmassoffset(my, vector(0, 0, -30), NULL);
  pXent_setmass(my, 100);
  ENTITY *FLwheel = ent_create("buggy_rad1.mdl", vector(my.x + 86, my.y - 43, my.z - 20), NULL);
  ENTITY *FRwheel = ent_create("buggy_rad1.mdl", vector(my.x + 86, my.y + 43, my.z - 20), NULL);
  ENTITY *BLwheel = ent_create("buggy_rad1.mdl", vector(my.x - 70, my.y + 43, my.z - 20), NULL);
  ENTITY *BRwheel = ent_create("buggy_rad1.mdl", vector(my.x - 70, my.y - 43, my.z - 20), NULL);
  pXcon_add(PH_WHEEL, FLwheel, me, 0);
  pXcon_add(PH_WHEEL, FRwheel, me, 0);
  pXcon_add(PH_WHEEL, BLwheel, me, 0);
  pXcon_add(PH_WHEEL, BRwheel, me, 0);
  temp.x = 10000;
  temp.y = 500;
  temp.z = 6;

  temp2.x = 500;
  temp2.y = 100;
  temp2.z = 0;
  pXcon_setparams2(FLwheel, NULL, temp2, temp);
  pXcon_setparams2(FRwheel, NULL, temp2, temp);
  pXcon_setparams2(BLwheel, NULL, temp2, temp);
  pXcon_setparams2(BRwheel, NULL, temp2, temp);
  set(FLwheel, INVISIBLE);
  set(FRwheel, INVISIBLE);
  set(BLwheel, INVISIBLE);
  set(BRwheel, INVISIBLE);

  vec_set(temp, vnode2);
  vec_sub(temp, my.x);
  ANGLE temp3;
   temp3.pan= my.pan;
  vec_to_angle(temp3.pan, temp);
  temp3.roll=0;
  temp3.tilt=0;
  pXent_rotate(my, NULL, temp3);
  pXent_addforcelocal(me, nullvector, _vec(5000, 0, 0));
  STRING *debugs = "1";
  ANGLE vangle;
  ANGLE v3angle;
  var yp;
  var brake;
  wait(1);
  var carai_engine = ent_playloop(my, carai1_wav, 700);

  while (my.alpha < 100) {
    my.alpha += 1;
    pXent_addforcelocal(me, _vec(100 * cos(my.pan), 100 * sin(my.pan), 0), _vec(100, 0, 0));
    wait(1);
  }
  my.alpha=100;
  reset(my, TRANSLUCENT);
  wait(1);
  AssignEnvMap(my);
  var lanim, lamin2;
  while (1) {

    //	camera.x=my.x;
    //	camera.y=my.y;

    if (vec_dist(my.x, player.x) > 6000) {
      my.skill20 = 0;
      wait(60);
      ent_remove(me);
      FTrafficCount -= 1;
      return;
    }

    if (vec_dist(my.x, vnode1) > 300) {
      accel = 2050;
    } else {
      node_num1 = node_num2;
      path_getnode(my, node_num1, vnode1, nodeskills);

      node_num2 = node_num3;
      path_getnode(my, node_num2, vnode2, nodeskills);

      node_num3 = node_num4;
      path_getnode(my, node_num3, vnode3, nodeskills);

      node_num4 = GetNextNode(node_num3, nodeskills);
      path_getnode(my, node_num4, vnode4, nodeskills);
    }
    // calculate bezier curve from my.x, node1 and node2 to get some smooth steering
    vec_set(tempv, my.x);
    vec_add(tempv, vnode2);
    vec_scale(tempv, 0.5);

    vec_add(tempv, vnode1);
    vec_scale(tempv, 0.5);

    // if AI to close to nodes, then get next nodes
    if (vec_dist(my.x, tempv) < 300) {
      node_num1 = node_num2;
      path_getnode(my, node_num1, vnode1, nodeskills);

      node_num2 = node_num3;
      path_getnode(my, node_num2, vnode2, nodeskills);

      node_num3 = node_num4;
      path_getnode(my, node_num3, vnode3, nodeskills);

      node_num4 = GetNextNode(node_num3, nodeskills);
      path_getnode(my, node_num4, vnode4, nodeskills);
    }

    if (vec_dist(my.x, vnode2) < 300) {
      node_num1 = node_num2;
      path_getnode(my, node_num1, vnode1, nodeskills);

      node_num2 = node_num3;
      path_getnode(my, node_num2, vnode2, nodeskills);

      node_num3 = node_num4;
      path_getnode(my, node_num3, vnode3, nodeskills);

      node_num4 = GetNextNode(node_num3, nodeskills);
      path_getnode(my, node_num4, vnode4, nodeskills);
    }

    if (vec_dist(my.x, vnode3) < 300) {
      node_num1 = node_num2;
      path_getnode(my, node_num1, vnode1, nodeskills);

      node_num2 = node_num3;
      path_getnode(my, node_num2, vnode2, nodeskills);

      node_num3 = node_num4;
      path_getnode(my, node_num3, vnode3, nodeskills);

      node_num4 = GetNextNode(node_num3, nodeskills);
      path_getnode(my, node_num4, vnode4, nodeskills);
    }

    //	draw_line3d(tempv,NULL,100); // move to first corner
    //	draw_line3d(my.x,vector(0,255,0),100);
    //	draw_line3d(vnode1,vector(255,0,0),100);
    //	draw_line3d(vnode2,vector(255,0,0),100);
    //	draw_line3d(vnode3,vector(255,0,0),100);
    //	draw_line3d(vnode4,vector(255,0,0),100);

    temp.x = accel * cosv(my.pan);
    temp.y = accel * sinv(my.pan);
    temp.z = 0;

    vec_set(tempang, tempv);
    vec_sub(tempang, my.x);
    vec_to_angle(vangle, tempang);

    vec_set(tempang, vnode4);
    vec_sub(tempang, my.x);
    vec_to_angle(v3angle, tempang);

    steer = 0;

    if (v3angle.pan - my.pan > 180) {
      v3angle.pan = v3angle.pan - 360;
    }
    if (my.pan - v3angle.pan > 180) {
      v3angle.pan = v3angle.pan + 360;
    }

    if (vangle.pan - my.pan > 180) {
      vangle.pan = vangle.pan - 360;
    }
    if (my.pan - vangle.pan > 180) {
      vangle.pan = vangle.pan + 360;
    }

    if (vangle.pan > my.pan) {
      steer = clamp(vangle.pan - my.pan, 0, 20);
    } else {
      steer = clamp(vangle.pan - my.pan, -20, 0);
    }

    if (v3angle.pan > my.pan) {
      brake = v3angle.pan - my.pan;
    } else {
      brake = v3angle.pan - my.pan;
    }

    // if 1st node have big angle, AI slow down
    if (abs(steer) > 10) {
      accel = 1550;
    }
    if (abs(steer) > 15) {
      accel = 1050;
    }
    if (abs(steer) > 19) {
      accel = 950;
    }

    temp.x = accel * cosv(my.pan);
    temp.y = accel * sinv(my.pan);
    temp.z = 0;

    pXent_getvelocity(my, temp, nullvector);
    v = vec_dist(temp, nullvector) / 10;
    my.skill1 = v;
    snd_tune(carai_engine, 0, v, 0);
    lanim = lanim + v / 300;

    // if 3rd path node have big angle, AI will slow down or stop
    if (abs(brake) > 10) {
      if (v > 20) {
        accel = 800;
      } else {
        accel = 1550;
      }
      if (v > 40) {
        accel = -800;
      }
    }

    str_cpy(debugs, ""); // clear the string
    z = c_scan(my.x, my.pan, vector(25, 80, 1000), IGNORE_WORLD | IGNORE_ME | SCAN_FLAG2);
    if (you) {
      p = my.pan;
      yp = you.pan;

      if (p < 0) {
        p += 360;
      }
      if (yp < 0) {
        yp += 360;
      }

      if (p > 360) {
        p -= 360;
      }
      if (yp > 360) {
        yp -= 360;
      }

      if ((yp < 30) && (p > 330)) {
        yp += 180;
        p -= 180;
      }

      if ((p < 30) && (yp > 330)) {
        p += 180;
        yp -= 180;
      }

      // if AI saw something with +- same pan...
      if (abs(p - yp) < 30) {
        //	draw_line3d(my.x,NULL,100);
        //	draw_line3d(you.x,vector(0,0,255),100);

        accel = 0;
        if (v > 10) // and AI have velocity >10
        {
          if (z < 800) // and it near than 800 quants, AI must stop
          {
            accel = -10000;
            str_cat(debugs, "BRAKE!");
          }
        } else {
          if (z < 500) {
            if (z < 400) {
              if (v >= 1) // if we have velocity <10 and >1, AI will slowdown
              {
                accel = -800;
                str_cat(debugs, "brake");
              }
            } else {
              if (v < 2) // if AI stoped to early, AI can add some speed
              {
                accel = 1800;
                str_cat(debugs, "go");
              }
            }
          } else {
            if (v < 4) // if AI stoped to early, AI can add some speed
            {
              accel = 800;
              str_cat(debugs, "go");
            }
          }
        }
      } else // if AI seen another car at another angle with velosity > 5, AI will slowdown
      {
        if (is(you, FLAG1)) {
          if (you.skill1 > 5) {
            if (v >= 1) {
              accel = -800;
              str_cat(debugs, "brake pass");
            }
          }
        }
      }

      temp.z += 100;
      vec_set(temp, my.x);
      //	if (vec_to_screen(temp,camera))
      //	draw_text(debugs,temp.x,temp.y,vector(100,100,255));
    }
    // if another car is near on the right side, AI will pass it
    z = c_scan(my.x, my.pan - 90, vector(160, 180, 150), IGNORE_ME | SCAN_FLAG2);
    if (you) {
      temp.z += 100;
      vec_set(temp, my.x);
      if (v > 5) {
        accel = -1800;
      }
      str_cat(debugs, "pass");
      //	if (vec_to_screen(temp,camera))
      //	draw_text(debugs,temp.x,temp.y,vector(100,100,255));
    }

    if (accel < 0) {
      lamin2 = 1;
    } else {
      lamin2 = 0;
    }
    if (lanim > 2) {
      lanim = 0;
    }
    my.skin = 1 + (lamin2 * 2) + lanim;

    pXcon_setwheel(FLwheel, -steer, 0, 0);
    pXcon_setwheel(FRwheel, -steer, 0, 0); // steer to the right
    pXcon_setwheel(BLwheel, 0, -accel, 0);
    pXcon_setwheel(BRwheel, 0, -accel, 0);

    wait(1);
  }
}

action ATrafficSpawner() {
  while (!player) {
    wait(1);
  }
  var node_num1 = 1;
  var node_num2;
  var v;
  var z = 999999;
  var node_num3;
  var node_num4;
  var node_count = 1;
  VECTOR tempv;
  VECTOR tempang;
  VECTOR vnode1;
  VECTOR vnode2;
  VECTOR vnode3;
  FTrafficCount =0;
  VECTOR vnode4;
  var nodeskills[6];
  wait(-1);
  var ltypecar;
  while (FDestroyMe==1) {
    if (FTrafficCount < 20) {
      my.x = player.x + (random(10000)) - 5000;
      my.y = player.y + (random(10000)) - 5000;
      my.z = player.z;
      wait(1);
      while (vec_dist(my.x, player.x) < 4000) {
        my.x = player.x + (random(10000)) - 5000;
        my.y = player.y + (random(10000)) - 5000;
        my.z = player.z;
        wait(1);
      }
      node_num1 = 1;
      z = 999999;
      while (path_getnode(my, node_num1, vnode1, nodeskills) != 0) {
        v = vec_dist(my.x, vnode1);
        if (v <= z) {
          z = v;
          node_count = node_num1;
        }
        node_num1 += 1;
      }

      z = 0;
      node_num1 = node_count;
      path_getnode(my, node_num1, vnode1, nodeskills);
      vnode1.z += 40;
      you = NULL;
      if (vec_dist(vnode1, player.x) > 2500) {
        c_scan(vnode1, my.pan, vector(360, 0, 800), SCAN_FLAG2);
        if (!you) {
          ltypecar = floor(random(4));

          var dist_down;
          if (c_trace(vnode1.x, vector(vnode1.x, vnode1.y, vnode1.z - 5000), IGNORE_ME | IGNORE_PASSABLE | USE_POLYGON) > 0)
            dist_down = vnode1.z - target.z; // get distance between player's feet and the ground
          else
            dist_down = 0;
          vnode1.z = vnode1.z - abs(dist_down) + 40;

          if (ltypecar < 1) {
            ent_create("350z.mdl", vnode1, ACar_Traffic);
          } else if (ltypecar < 2) {
            ent_create("cap.mdl", vnode1, ACar_Traffic);
          } else if (ltypecar < 3) {
            ent_create("celica.mdl", vnode1, ACar_Traffic);          
          } else {
            ent_create("rsx.mdl", vnode1, ACar_Traffic);
          }
        }
      }
    }
    wait(-1);
  }
}

// if you set skill1==1, the traffic light will work first half of sys_minute, else second half
// if you set FLAG3, this will automaticly set or reset FLAG2 and AI will recognize lights
// lights MUST have pan direction along the AI path, or AI ignore it
// if you set FLAG1, AI recognize lights at any pan angle
action ATrafficLight() {
  set(my, LIGHT);
  if (my.skill1 == 1) {
    while (1) {
      if (sys_seconds < 60) {
        my.frame = 3;
      }
      if (sys_seconds < 59) {
        my.frame = 1;
      }
      if (sys_seconds < 58) {
        my.frame = 4;
      }
      if (sys_seconds < 57) {
        my.frame = 1;
      }
      if (sys_seconds < 56) {
        my.frame = 4;
      }
      if (sys_seconds < 30) {
        my.frame = 3;
      }
      if (sys_seconds < 28) {
        my.frame = 2;
      }
      if (is(my, FLAG3)) {
        if (my.frame == 2) {
          set(my, FLAG2);
        } else {
          reset(my, FLAG2);
        }
      }
      wait(-1);
    }
  } else {
    while (1) {
      if (sys_seconds < 60) {
        my.frame = 3;
      }
      if (sys_seconds < 59) {
        my.frame = 2;
      }
      if (sys_seconds < 28) {
        my.frame = 3;
      }
      if (sys_seconds < 27) {
        my.frame = 1;
      }
      if (sys_seconds < 26) {
        my.frame = 4;
      }
      if (sys_seconds < 25) {
        my.frame = 1;
      }
      if (sys_seconds < 24) {
        my.frame = 4;
      }
      if (is(my, FLAG3)) {
        if (my.frame == 2) {
          set(my, FLAG2);
        } else {
          reset(my, FLAG2);
        }
      }
      wait(-1);
    }
  }
}

// player action
action ACarRotate() {
  AssignStaticEnvMap(my);
  while (FSelect == 0) {
    wait(1);
    my.pan = my.pan + 1 * time_step;
  }
  my.skill20 = 0;

}

action ACar() {
  wait(1);
  my.skill19 = 2;
  path_set(me, "path_000");
  tcamera_mode = 1;
  tcamera_dist = 450;
  tcamera_height = 150;
  tcamera_minDist = 500;
  tcamera_maxDist = 2000;
  tcamera_zoomDist = 2;
  camera_startup();
ANGLE temp3;
  set(my, FLAG2);
  set(my, FLAG1);
  AssignEnvMap(my);
  var accel = 0;
  var steer = 0;
  VECTOR temp;
  var v, z;
  actor(PH_CONVEX);
  pXent_setfriction(my, 50);
  player = me;
  VECTOR temp2;
  pXent_setmassoffset(my, vector(0, 0, -30), NULL);
  pXent_setmass(my, 100);

  ENTITY *FLwheel = ent_create("buggy_rad1.mdl", vector(my.x + 86, my.y - 43, my.z - 20), NULL);
  ENTITY *FRwheel = ent_create("buggy_rad1.mdl", vector(my.x + 86, my.y + 43, my.z - 20), NULL);
  ENTITY *BLwheel = ent_create("buggy_rad1.mdl", vector(my.x - 70, my.y - 43, my.z - 20), NULL);
  ENTITY *BRwheel = ent_create("buggy_rad1.mdl", vector(my.x - 70, my.y + 43, my.z - 20), NULL);

  pXent_setfriction(FLwheel, 90);
  pXent_setfriction(FRwheel, 90);
  pXent_setfriction(BLwheel, 90);
  pXent_setfriction(BRwheel, 90);

  pXcon_add(PH_WHEEL, FLwheel, me, 0);
  pXcon_add(PH_WHEEL, FRwheel, me, 0);
  pXcon_add(PH_WHEEL, BLwheel, me, 0);
  pXcon_add(PH_WHEEL, BRwheel, me, 0);

  temp.x = 1;
  temp.y = 0;
  temp.z = 0;

  temp2.x = 0.1;
  temp2.y = -0.1;
  temp2.z = -1;
  pXcon_setparams1(FLwheel, NULL, vec_normalize(temp2, 1), temp);

  temp2.x = 0.1;
  temp2.y = -0.1;
  temp2.z = -1;
  pXcon_setparams1(BLwheel, NULL, vec_normalize(temp2, 1), temp);

  temp2.x = -0.1;
  temp2.y = 0.1;
  temp2.z = -1;
  pXcon_setparams1(FRwheel, NULL, vec_normalize(temp2, 1), temp);

  temp2.x = -0.1;
  temp2.y = 0.1;
  temp2.z = -1;
  pXcon_setparams1(BRwheel, NULL, vec_normalize(temp2, 1), temp);

  // use this to setup px settings
  temp.x = 2000; // wheel suspension spring constant (default: 1000). Adjust this to the car mass and gravity.
  temp.y = 200;  // wheel suspension damper constant (default: 100).
  temp.z = 6;    // wheel suspension way in quants (default: wheel radius).
  temp2.x = 100; // wheel longitudinal friction curve scale factor in percent (default = 100).
  temp2.y = 70;  // wheel lateral friction curve scale factor in percent (default = 100). Set this to a high value for vehicles on rails.
  temp2.z = 0;   // not used yet.

  pXcon_setparams2(FLwheel, NULL, temp2, temp);
  pXcon_setparams2(FRwheel, NULL, temp2, temp);
  pXcon_setparams2(BLwheel, NULL, temp2, temp);
  pXcon_setparams2(BRwheel, NULL, temp2, temp);

  set(FLwheel, INVISIBLE);
  set(FRwheel, INVISIBLE);
  set(BLwheel, INVISIBLE);
  set(BRwheel, INVISIBLE);
  wait(1);

  var carai_engine = ent_playloop(my, carai1_wav, 10000);
  var lsteer = 0, lsteeror = 0;
  var lbrake;
  var lanim = 0; // pXent_addforcelocal( me, _vec(5000*cos(my.pan),5000*sin(my.pan),0) , _vec(100,0,0));
  while (1) {
    accel = (key_w - key_s) * 10000;

    temp.x = accel * cosv(my.pan) - 0 * sinv(my.pan);
    temp.y = 0 * cosv(my.pan) + accel * sinv(my.pan);
    temp.z = 0;
    //
    pXent_getvelocity(my, temp, nullvector);
    v = vec_dist(temp, nullvector) / 10;
    my.skill1 = v;
    snd_tune(carai_engine, 0, v, 0);
    tcamera_dist = 450 + v * 2;
    steer = -(key_a - key_d);
    lsteeror = steer * (30 - clamp(v / 4, 0, 15));

    lanim = lanim + v / 300;
    if (lanim > 2) {
      lanim = 0;
    }

    my.skin = 1 + (key_s * 2) + lanim;

    if (lsteer < lsteeror) {
      lsteer += 10 * time_step;
    } else if (lsteer > lsteeror) {
      lsteer -= 10 * time_step;
    }
    if (v > 10) {
      if (accel < 0) {
        lbrake = -accel;
      } else {
        lbrake = 0;
      }
    } else {
      lbrake = 0;
    }

    if (my.skill19 == 12) {
      accel = 0;
    } else if ((my.skill19 == 11) || (my.skill19 == 1) || (my.skill19 == 6)) {
      if (vec_dist(my.x, FCheck1.x) < 1000) {
        my.skill19 = my.skill19 + 1;
      } else {
        my.skill18 = vec_dist(my.x, FCheck1.x);
      }
    } else if ((my.skill19 == 2) || (my.skill19 == 7)) {
      if (vec_dist(my.x, FCheck2.x) < 1000) {
        my.skill19 = my.skill19 + 1;
      } else {
        my.skill18 = vec_dist(my.x, FCheck2.x);
      }
    } else if ((my.skill19 == 3) || (my.skill19 == 8)) {
      if (vec_dist(my.x, FCheck3.x) < 1000) {
        my.skill19 = my.skill19 + 1;
      } else {
        my.skill18 = vec_dist(my.x, FCheck3.x);
      }
    } else if ((my.skill19 == 4) || (my.skill19 == 9)) {
      if (vec_dist(my.x, FCheck4.x) < 1000) {
        my.skill19 = my.skill19 + 1;
      } else {
        my.skill18 = vec_dist(my.x, FCheck4.x);
      }
    } else

        if ((my.skill19 == 5) || (my.skill19 == 10)) {
      if (vec_dist(my.x, FCheck5.x) < 1000) {
        my.skill19 = my.skill19 + 1;
      } else {
        my.skill18 = vec_dist(my.x, FCheck5.x);
      }
    }

    if (my.skill19 < 7) {
      lap = 1;
    } else {
      lap = 2;
    }

    if (key_r) {
      var v = 0;
      var z = 9999999;
      var node_num1=56;
      var node_count;
      var nodeskills;
      VECTOR vnode1;

      while ((path_getnode(my, node_num1, vnode1, nodeskills) != 0)&&(node_num1<106)) {
  
        v = vec_dist(my.x, vnode1);
        if (v <= z) {
          z = v;
          node_count = node_num1;
        }
        node_num1 += 1;
      }
  
      path_getnode(my, node_count, vnode1, nodeskills);
      
      pXent_enable(me, 0);
      pXent_enable(FLwheel, 0);
      pXent_enable(FRwheel, 0);
      pXent_enable(BLwheel, 0);
      pXent_enable(BRwheel, 0);

      my.x = vnode1.x;
      my.y = vnode1.y;
      my.z = vnode1.z;
      my.roll = 0;
      my.tilt = 0;
      my.pan = 0;

      FLwheel.roll = 0;
      FLwheel.tilt = 0;
      FLwheel.pan = 0;
      FLwheel.x = my.x + 86;
      FLwheel.y = my.y - 43;
      FLwheel.z = my.z - 20;

      FRwheel.roll = 0;
      FRwheel.tilt = 0;
      FRwheel.pan = 0;
      FRwheel.x = my.x + 86;
      FRwheel.y = my.y + 43;
      FRwheel.z = my.z - 20;

      BLwheel.roll = 0;
      BLwheel.tilt = 0;
      BLwheel.pan = 0;
      BLwheel.x = my.x - 70;
      BLwheel.y = my.y + 43;
      BLwheel.z = my.z - 20;

      BRwheel.roll = 0;
      BRwheel.tilt = 0;
      BRwheel.pan = 0;
      BRwheel.x = my.x - 70;
      BRwheel.y = my.y - 43;
      BRwheel.z = my.z - 20;

      pXent_enable(me, 1);

      pXent_setmassoffset(my, vector(0, 0, -30), NULL);
      pXent_setmass(my, 100);
      pXent_setfriction(my, 50);
      
  pXent_enable(FLwheel, 1);
      pXent_enable(FRwheel, 1);
      pXent_enable(BLwheel, 1);
      pXent_enable(BRwheel, 1);
      
      
      pXent_setfriction(FLwheel, 90);
  pXent_setfriction(FRwheel, 90);
  pXent_setfriction(BLwheel, 90);
  pXent_setfriction(BRwheel, 90);

  pXcon_add(PH_WHEEL, FLwheel, me, 0);
  pXcon_add(PH_WHEEL, FRwheel, me, 0);
  pXcon_add(PH_WHEEL, BLwheel, me, 0);
  pXcon_add(PH_WHEEL, BRwheel, me, 0);

  temp.x = 1;
  temp.y = 0;
  temp.z = 0;

  temp2.x = 0.1;
  temp2.y = -0.1;
  temp2.z = -1;
  pXcon_setparams1(FLwheel, NULL, vec_normalize(temp2, 1), temp);

  temp2.x = 0.1;
  temp2.y = -0.1;
  temp2.z = -1;
  pXcon_setparams1(BLwheel, NULL, vec_normalize(temp2, 1), temp);

  temp2.x = -0.1;
  temp2.y = 0.1;
  temp2.z = -1;
  pXcon_setparams1(FRwheel, NULL, vec_normalize(temp2, 1), temp);

  temp2.x = -0.1;
  temp2.y = 0.1;
  temp2.z = -1;
  pXcon_setparams1(BRwheel, NULL, vec_normalize(temp2, 1), temp);

  // use this to setup px settings
  temp.x = 2000; // wheel suspension spring constant (default: 1000). Adjust this to the car mass and gravity.
  temp.y = 200;  // wheel suspension damper constant (default: 100).
  temp.z = 6;    // wheel suspension way in quants (default: wheel radius).
  temp2.x = 100; // wheel longitudinal friction curve scale factor in percent (default = 100).
  temp2.y = 70;  // wheel lateral friction curve scale factor in percent (default = 100). Set this to a high value for vehicles on rails.
  temp2.z = 0;   // not used yet.

  pXcon_setparams2(FLwheel, NULL, temp2, temp);
  pXcon_setparams2(FRwheel, NULL, temp2, temp);
  pXcon_setparams2(BLwheel, NULL, temp2, temp);
  pXcon_setparams2(BRwheel, NULL, temp2, temp);
  

      
      path_getnode(my, node_count+1, vnode1, nodeskills);
      
 
      vec_set(temp, vnode1);
      vec_sub(temp, my.x);
       
       temp3.pan = my.pan;
      vec_to_angle(temp3.pan, temp);
      temp3.tilt=0;
      temp3.roll=0;
      pXent_rotate(my, NULL, temp3);
      wait(-1);
      
      
    }

    pXcon_setwheel(FLwheel, floor(lsteer) - 1, 0, lbrake);
    pXcon_setwheel(FRwheel, floor(lsteer) + 1, 0, lbrake);
    pXcon_setwheel(BLwheel, -1, accel / ((v / 50) + 1), lbrake + key_space * 4000);
    pXcon_setwheel(BRwheel, 1, accel / ((v / 50) + 1), lbrake + key_space * 4000);

    wait(1);
  }
}

VIEW *camera2_view = {layer = 1;
pos_x = 0;
pos_y = 0;
size_x = 256;
size_y = 256;
arc = 45;
aspect = 1;
ambient = 10;
fog = 10;
genius = NULL;
}

function main() {
  var vidhandle;
  var vidhandle2;
  var FSel2;
  physX_open();
  vec_set(sky_color, vector(10, 1, 1)); // dark blue
  video_window(NULL, NULL, 0, "Dorifto Samurai");
  d3d_antialias = 9;
  shadow_stencil = 0;
  video_mode = 12;
  wait(1);
 // video_screen = 1;
 
  
  loser_panel.pos_y = screen_size.y/2;
  loser_panel.pos_x = (screen_size.x/2)-200;
  
   winner_panel.pos_y = screen_size.y/2;
  winner_panel.pos_x = (screen_size.x/2)-250;

start:
 FDestroyMe=1;
FCar1=NULL;
FCar2=NULL;
  FSelect = 0;
  tcamera_mode = 0;
  set(my_panel2, SHOW);
  reset(my_panel, SHOW);

  //  video_screen = 1;
  vidhandle = media_loop("3.mp3", NULL, 100);

  level_load("2.wmb");
  wait(1);
  camera.x = 0;
  camera.y = 0;
  camera.z = 100;
  camera.tilt = -10;
  camera.pan = 270;

  var LTarget = 0;
  while (FSelect == 0) {
    if (key_a) {
      LTarget = camera.pan - 90;
      while (camera.pan != LTarget) {
        camera.pan -= 1;
        wait(1);
      }
    }
    if (key_d) {
      LTarget = camera.pan + 90;
      while (camera.pan != LTarget) {
        camera.pan += 1;
        wait(1);
      }
    }
    vg = camera.pan;
    if (camera.pan >= 360) {
      camera.pan -= 360;
    }
    if (camera.pan < 0) {
      camera.pan += 360;
    }
    if (key_enter) {
      FSelect = 1;
    }
    wait(1);
  }
  media_play("aicarstart.wav", NULL, 100);
  wait(-1);

  if (camera.pan < 89) {
    FSelect = 1;
  } else if (camera.pan < 179) {
    FSelect = 2;
  } else if (camera.pan < 260) {
    FSelect = 3;
  } else {
    FSelect = 4;
  }
  reset(my_panel2, SHOW);
  set(my_panel, SHOW);
  media_stop(vidhandle);
  vidhandle = media_play("intro.wmv", NULL, 100);
   FDestroyMe=1;
  wait(-2);
  level_load("1.wmb");
  if (FSelect == 4) {
    ent_create("rsx.mdl", _vec(11650, 26868, 885), ACar);
  }
  if (FSelect == 3) {
    ent_create("cap.mdl", _vec(11650, 26868, 885), ACar);
  }
  if (FSelect == 2) {
    ent_create("350z.mdl", _vec(11650, 26868, 885), ACar);
  }
  if (FSelect == 1) {
    ent_create("celica.mdl", _vec(11650, 26868, 885), ACar);
  }
  
  
  random_seed(0);
  FSel2 = floor(random(4)+1);
  
  if (FSel2 == 4) {
    ent_create("rsx.mdl", _vec(12218, 26965, 885), ACar_Enemy);
  }
  if (FSel2 == 3) {
    ent_create("cap.mdl", _vec(12218, 26965, 885), ACar_Enemy);
  }
  if (FSel2 == 2) {
    ent_create("350z.mdl", _vec(12218, 26965, 885), ACar_Enemy);
  }
  if (FSel2 == 1) {
    ent_create("celica.mdl", _vec(12218, 26965, 885), ACar_Enemy);
  }
  random_seed(0);
  FSel2 = floor(random(4)+1);
  
  if (FSel2 == 4) {
    ent_create("rsx.mdl", _vec(15734, 26978, 885), ACar_Enemy);
  }
  if (FSel2 == 3) {
    ent_create("cap.mdl", _vec(15734, 26978, 885), ACar_Enemy);
  }
  if (FSel2 == 2) {
    ent_create("350z.mdl", _vec(15734, 26978, 885), ACar_Enemy);
  }
  if (FSel2 == 1) {
    ent_create("celica.mdl", _vec(15734, 26978, 885), ACar_Enemy);
  }
  

  wait(-1);
  while (media_playing(vidhandle) != 0) {
    wait(1);
  }
  set(camera2_view, SHOW);
  camera2_view.pos_y = screen_size.y - 280;
  camera2_view.pos_x = 24;

  if (FSelect == 4) {
    vidhandle2 = media_loop("2.mp3", NULL, 100);
  } else {
    vidhandle2 = media_loop("1.mp3", NULL, 100);
  }

  pXent_settype(NULL, PH_STATIC, PH_PLANE);
  pX_setgravity(vector(0, 0, -9.81));

  while (player.skill19 < 12) {
    wait(1);
    camera2_view.x = player.x;
    camera2_view.y = player.y;
    camera2_view.z = 10000;
    camera2_view.pan = player.pan;
    camera2_view.tilt = -90;
 
    if (player.skill19 > FCar1.skill19) {
      if (player.skill19 > FCar2.skill19) {
        vg = 1;
      } else if (player.skill19 < FCar2.skill19) {
        vg = 2;
      } else if (player.skill18 < FCar2.skill18) {
        vg = 1;
      } else {
        vg = 2;
      }
    } else if (player.skill19 < FCar1.skill19) {
      if (player.skill19 > FCar2.skill19) {
        vg = 2;
      } else if (player.skill19 < FCar2.skill19) {
        vg = 3;
      } else if (player.skill18 < FCar2.skill18) {
        vg = 2;
      } else {
        vg = 3;
      }
    } else {
      if (player.skill18 < FCar1.skill18) {
        if (player.skill19 > FCar2.skill19) {
          vg = 1;
        } else if (player.skill19 < FCar2.skill19) {
          vg = 2;
        } else if (player.skill18 < FCar2.skill18) {
          vg = 1;
        } else {
          vg = 2;
        }
      } else {
        if (player.skill19 > FCar2.skill19) {
          vg = 2;
        } else if (player.skill19 < FCar2.skill19) {
          vg = 3;
        } else if (player.skill18 < FCar2.skill18) {
          vg = 2;
        } else {
          vg = 3;
        }
      }
    }
  }

  if (vg == 1) {
    set(winner_panel, SHOW);
  } else {
    set(loser_panel, SHOW);
  }
  
  wait(-5);
  media_stop(vidhandle2);
  reset(camera2_view, SHOW);
  reset(winner_panel, SHOW);
  reset(loser_panel, SHOW);
  FDestroyMe=0;
  wait(-1);
  goto start;
}