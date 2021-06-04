//*******************************************************//
//						MyNoise.h								//
//*******************************************************//

#ifndef _MYNOISE_H
#define  _MYNOISE_H

#include "MyAll.h"



extern int Noise1Value;
extern int Noise2Value;

extern int Noise1DB;
extern int Noise2DB;

extern int NoiseDB;

extern u8 NoiseFaultFun;
extern int AudioNdOffset;







void NoiseInit(void);



void Noise10ms(void);

void Noise100ms(void);


int NoiseGetDB(int adv);

int NoiseGetMyDB(void);

int NoiseCalculateOtherDB(int noise_db, int noise_mydb);




int NoiseNewProsess(void);

int NoiseNewGetMyDB(void);






#endif


