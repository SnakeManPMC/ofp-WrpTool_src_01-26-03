#include <stdio.h>
#include <stdlib.h>
#include <string.h>

//class for bmp file managing
#include "bmpmanclass.h"

int main()
{
	FILE *map;
	FILE *outp;
	FILE *text;
	FILE *vege;
	char sig[33],temp[80],*token,Rnd3dObjN[255][76];
	char dObjName[76];
	int ver;
	short hoss1,hoss2,hoss3;
	float dDir,dDirX=0,dDirZ=0;
	long dObjIndex;
	int amount=256*256,rx=0,rnd3d=0;


	map=fopen("terrain.txt","rt");
	if (!map) {printf("error in terrain.txt\n");exit(1);}

	outp=fopen("pmcworld.wrp","wb");
	if (!outp) {printf("error in pmcworld.wrp\n");exit(1);}

	text=fopen("wrpdebug.txt","wt");
	if (!text) {printf("error at wrpdebug.txt\n");exit(1);}

	/* vegetation configuration file & strings */
	vege=fopen("vegetation.txt","rt");
	if (!vege) {printf("error at vegetation.txt!\n"); exit(1);}

		fgets(Rnd3dObjN[rx],80,vege);

	while (!feof(vege)) {

		Rnd3dObjN[rx][strlen(Rnd3dObjN[rx])-1]=0;
		rx++;
		fgets(Rnd3dObjN[rx],80,vege);

	}

	fclose(vege);
	rx--;
	fprintf(text, "amount of strings in vegetation.txt: %i\n",rx);


	/* - signature - 4 bytes */
	sprintf(sig,"%s","4WVR");
	fwrite(sig,4,1,outp);
	fprintf(text, "sig: %s\n",sig);

	/* X size 256 */		
	ver=256;
	fwrite(&ver,4,1,outp);
	fprintf(text, "x size: %d\n",ver);

	/* Y size 256 */		
	fwrite(&ver,4,1,outp);
	fprintf(text, "y size: %d\n",ver);

	/* read Wilbur long/lat txt format elevations */
	//translates to OFP format: 
	//X, Z, Y
	for (int c1=0; c1 < amount; c1++) {

		fgets(temp,80,map);
        token=strtok(temp," ");
        sscanf(token,"%d",&hoss1);
        token=strtok(NULL," ");
        sscanf(token,"%d",&hoss2);
        token=strtok(NULL," ");
        sscanf(token,"%d",&hoss3);

//	fprintf(text,"%d %d %d\n",hoss1,hoss2,hoss3);
	hoss3=hoss3*6;
	fwrite(&hoss3,2,1,outp);
}

	/* texture indexes in short */
	hoss1=5;
	for (int ic=0; ic < amount; ic++) {

	fwrite(&hoss1,2,1,outp);
	fprintf(text, "texture index: %d\n",hoss1);
	}

	/* texture indexes in short */
	/* lets insert base textures */
	// landtext\s3.paa LandText\mo.pac
	sprintf(sig,"LandText\\mo.pac");
	fwrite(sig,32,1,outp);
	sprintf(sig,"landtext\\pi.paa");
	fwrite(sig,32,1,outp);
	sprintf(sig,"landtext\\s3.paa");
	fwrite(sig,32,1,outp);
	sprintf(sig,"landtext\\p4.paa");
	fwrite(sig,32,1,outp);
	sprintf(sig,"landtext\\bah.paa");
	fwrite(sig,32,1,outp);
	sprintf(sig,"landtext\\tt.paa");
	fwrite(sig,32,1,outp);

	/* textures 32 char length and total of 512 */
	for (int ix=0; ix < 506; ix++) {

	fwrite(sig,32,1,outp);
	fprintf(text, "texture string: %s\n",sig);
	}


/*
	3d objects!
	9 floats, x coord, elevation, z coord, 3d object index, 3d object filename
	1,0,0,0,1,0,0,0,1 floats
	x/z coords 0 to 12800 in 256 map
	elevation 32768 and its "fit to ground".
*/
	dObjIndex=0;

	for (int cx=0; cx < 200000; cx++) {
	dDir=1;
	fwrite(&dDir,4,1,outp);
	dDir=0;
	fwrite(&dDir,4,1,outp);
	fwrite(&dDir,4,1,outp);
	fwrite(&dDir,4,1,outp);
	dDir=1;
	fwrite(&dDir,4,1,outp);
	dDir=0;
	fwrite(&dDir,4,1,outp);
	fwrite(&dDir,4,1,outp);
	fwrite(&dDir,4,1,outp);
	dDir=1;
	fwrite(&dDir,4,1,outp);

	dDirX=1+(float) (12800.0*rand()/(RAND_MAX+1.0));
	dDirZ=1+(float) (12800.0*rand()/(RAND_MAX+1.0));
	
	fwrite(&dDirX,4,1,outp);
	dDir=32768;
	fwrite(&dDir,4,1,outp);
	fwrite(&dDirZ,4,1,outp);

	dObjIndex++;
	fwrite(&dObjIndex,4,1,outp);

	/* add the 3d objects randomly */
	rnd3d=1+(int) (rx*rand()/(RAND_MAX+1.0));
	sprintf(dObjName,Rnd3dObjN[rnd3d]);

	fwrite(dObjName,sizeof(dObjName),1,outp);
	fprintf(text,"%i 3dObj: %s in x: %f y: %f\n",rnd3d,dObjName,dDirX,dDirZ);
}

	fclose(map);
	fclose(outp);
	fclose(text);
	return 0;
}
