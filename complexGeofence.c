#include <stdio.h>
#include <stdlib.h>
#include <math.h>

int R=6371; // earth radius


/**
 * return: 1 if the point belongs to the projection of the polygon on the plane
 *         0 else
 */
int belongingPlan(int nvert, float *vertx, float *verty, float testx, float testy);

/**
 * return: 1 if the point to be tested belongs to the polygon by checking that it belongs to the three projections,
 *         0 else 
 */
int pnpolyLatLon(int nvert, float *vertx, float *verty, float *vertz, float testLat, float testLon);

/**
 * return: 1 if the point to be tested belongs to the projection of the polygon. 
 *         0 else
 */
int pnpoly(int nvert, float *vertx, float *verty, float testx, float testy);




/**
 * return: 0 if it is not a segment, but a plan
 *         1 if it is a segment and the point to be tested does NOT lie inside it
 *         2 if it is a segment and the point to be tested lies inside it
 */
int isASegment(int nvert, float *vertx, float *verty, float testx, float testy);

float valueXCartesian(float,float);
float valueYCartesian(float,float);
float valueZCartesian(float);


int main(void) {
  /*
  // case of a square
  float arrx[4] = {0,0,1,1};
  float arry[4] = {0,1,1,0};
  printf("%d",pnpoly(4,arrx,arry,0.5,0.5));
  */

  /*
  // case of a concave square
  float arrx[8] = {0,0,3,3,2,2,1,1};
  float arry[8] = {0,2,2,0,0,1,1,0};
  printf("%d",pnpoly(8,arrx,arry,2.5,0.5));
  */

  // lat and long conversions in x and y of the building
  float latitudes[6]={45.960291, 45.960261, 45.960250,45.960213,45.960126,45.960194};
  float longitudes[6]={12.329035, 12.329093,12.329083,12.329159,12.329070,12.328932};

  float arrX[6];

  float arrY[6];
  
  float arrZ[6];


  for(int i=0; i<6; i++){
    arrX[i]=valueXCartesian(latitudes[i],longitudes[i]);
    arrY[i]=valueYCartesian(latitudes[i],longitudes[i]);
    arrZ[i]=valueZCartesian(latitudes[i]);
    
    printf("The point %c has coordinates (%f,%f,%f)\n",i+65,arrX[i],arrY[i],arrZ[i]);
  }

  printf("\n*** Test phase ***\n");
  // ok case (G point)
  printf("G point:");
  if(pnpolyLatLon(6,arrX,arrY,arrZ,45.960197,12.328988)) printf("\t * belongs to the polygon."); else printf("\t * does not belong to the polygon.");
    

  // ko case (H point)
  printf("\n\nH point:");
  if(pnpolyLatLon(6,arrX,arrY,arrZ,45.960241, 12.329120)) printf("\t * belongs to the polygon."); else printf("\t * does not belong to the polygon.");
  
  printf("\n********************\n");

  return 0;
}


int pnpoly(int nvert, float *vertx, float *verty, float testx, float testy)
{
  int i, j, c = 0;
  for (i = 0, j = nvert-1; i < nvert; j = i++) {
    if ( ((verty[i]>testy) != (verty[j]>testy)) &&
	 (testx < (vertx[j]-vertx[i]) * (testy-verty[i]) / (verty[j]-verty[i]) + vertx[i]) )
       c = !c;
  }
  return c;
}


int pnpolyLatLon(int nvert, float *vertx, float *verty, float *vertz, float testLat, float testLon)
{
  float testx = valueXCartesian(testLat,testLon);
  float testy = valueYCartesian(testLat,testLon);
  float testz = valueZCartesian(testLat);
  printf("\n\t * ha coordinate (%f,%f,%f);\n",testx,testy,testz);
  
  return (
            // vertical projection, x y plan
            belongingPlan(nvert, vertx, verty, testx, testy) +
            // frontal projection, x z plan
            belongingPlan(nvert, vertx, vertz, testx, testz) +
            // lateral projection, y z plan
            belongingPlan(nvert, verty, vertz, testy, testz)
        )==3;
}


int isASegment(int nvert, float *vertx, float *verty, float testx, float testy){
    float fstVertX = vertx[0];
    float fstVertY = verty[0];
    float sndVertX = vertx[1];
    float sndVertY = verty[1];
    // verify the distance among the two points (they may overlap)
    int counter=2;
    
    // sndVertX and sndVertY change in case of coinciding points
    while(counter<nvert){
        if(sqrt(pow(fstVertX-sndVertX,2)+pow(fstVertY-sndVertY,2))==0){
            sndVertX=vertx[counter];
            sndVertY=verty[counter];
            counter++;
        }else{
            counter=nvert;
        }
    }
    
    
    int res=1;
    
    float m = 0;
    float q = 0;
    int verticalSegment=1;
    if((sndVertX-fstVertX)!=0){
        m=(sndVertY-fstVertY) / (sndVertX-fstVertX);
        q = (sndVertX*fstVertY-fstVertX*sndVertY)/(sndVertX-fstVertX);
        verticalSegment = 0;
    }
    
    for(int i=2; i<nvert;i++){
        
        // it is a polygon instead of a segment
        if((sndVertX-fstVertX)==0 && (vertx[i]-fstVertX)!=0){
            verticalSegment=0;
            res = 0;
        }else{
            // it is a polygon
            if(verty[i]!=(m*vertx[i]+q) && verticalSegment==0){
                res = 0;
            }
        }
        
    }
    
    
    if(res==1){
        // processing for the vertical segment
        if(verticalSegment){
            float minY=verty[0];
            float maxY=verty[0];
            for(int i=1; i<nvert;i++){
                if(verty[i]>maxY)
                    maxY = verty[i];
                if(verty[i]<minY)
                    minY = verty[i];
            }
            // the point belongs to the segment
            if(testy>minY && testy<maxY && testx==vertx[0])
                res = 2;
        }else{
            // processing for the horizontal segment or oblique segment
            if(testy==(m*testx+q)){
                
                float minX=vertx[0];
                float maxX=vertx[0];
                for(int i=1; i<nvert;i++){
                    if(vertx[i]>maxX)
                        maxX = vertx[i];
                    if(vertx[i]<minX)
                        minX = vertx[i];
                }
                // the point belongs to the segment
                if(testx>minX && testx<maxX)
                    res = 2;
            }
        }
    }
    
    
    
    return res;
}


int belongingPlan(int nvert, float *vertx, float *verty, float testx, float testy){
    int res=0;
    
    int resSegm = isASegment(nvert, vertx, verty, testx, testy);
    if(resSegm==0){
      res = pnpoly(nvert, vertx, verty, testx, testy);
    }else{
      res = resSegm-1;
    }
    
    return res;
}


float valueXCartesian(float lat,float lon){
  return R * cos(lat) * cos(lon);
}

float valueYCartesian(float lat,float lon){
  return R * cos(lat) * sin(lon);
}

float valueZCartesian(float lat){
  return R *sin(lat);
}

