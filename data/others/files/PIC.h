#ifndef PIC_H
#define PIC_H

#define PISIGNALDATA (6)
#define PISIGNALMASK ((1<<PISIGNALDATA)-1)

#define NUMPLATFORMS (8)
#define PLATFORMSIZE (256*3) //half

#define NUMAARS (300)

typedef enum
{
	PI_START=1,  	    //ARG : 0
	PI_PAUSE=2,  	    //ARG : 0
	PI_STOP=3,   	    //ARG : 0
	PI_RESET=4,  	    //ARG : 0
	PI_RESETALL=18,	    //ARG : 0
	PI_ADDBOX=5,  	    //ARG : 5 (id;[sizex|sizey][sizez|mass][posx][posy][posz])
	PI_APPLYFORCE=6,    //ARG : 5 (id;[posx|posy][posz][vx][vy][vz])
	PI_ADDAAR=7,  	    //ARG : 5 (id;[sizex][sizey][sizez][normal][posx][posy][posz])
	PI_MAKEGRID=8,      //ARG : 0
	PI_SETVELOCITY=9,   //ARG : 3 (id;[vx][vy][vz])
	PI_UPDATEPLAYER=10, //ARG : 3 ([vx][vy][vz])
	PI_UPDATEPORTAL=11, //ARG : 7 (id;[px][py][pz][n][p0x][p0y][p0z])
	PI_ADDPLATFORM=12,  //ARG : 3 (id;[posx][posy][posz])
	PI_UPDATEPLATFORM=13,//ARG : 3 (id;[posx][posy][posz])
	PI_TOGGLEPLATFORM=14,//ARG : 1 (id;[active])
	PI_KILLBOX=15,       //ARG : 0 (id)
	PI_RESETPORTALS=16,  //ARG : 0
	PI_TOGGLEAAR=17,  	 //ARG : 0 (id)
	// 18 taken you fool
}message_type;

// void generateGuidAAR(portal_struct* p);

// static inline void computePortalPlane(portal_struct* p)
// {
// 	if(!p)return;
	
// 	p->plane[1]=vectProduct(p->normal,p->plane[0]);
	
// 	//TEST
// 	generateGuidAAR(p);
// }

// static inline vect3D warpVector(portal_struct* p, vect3D v)
// {
// 	if(!p)return vect(0,0,0);
// 	portal_struct* p2=p->targetPortal;
// 	if(!p2)return vect(0,0,0);
	
// 	// computePortalPlane(p2);
	
// 	int32 x=dotProduct(v,p->plane[0]);
// 	int32 y=dotProduct(v,p->plane[1]);
// 	int32 z=dotProduct(v,p->normal);
	
// 	return addVect(vectMult(p2->normal,-z),addVect(vectMult(p2->plane[0],-x),vectMult(p2->plane[1],y)));
// }

// static inline void warpMatrix(portal_struct* p, int32* m) //3x3
// {
// 	if(!m)return;
	
// 	vect3D x=warpVector(p,vect(m[0],m[3],m[6]));
// 	vect3D y=warpVector(p,vect(m[1],m[4],m[7]));
// 	vect3D z=warpVector(p,vect(m[2],m[5],m[8]));
	
// 	m[0]=x.x;m[3]=x.y;m[6]=x.z;
// 	m[1]=y.x;m[4]=y.y;m[7]=y.z;
// 	m[2]=z.x;m[5]=z.y;m[8]=z.z;
// }

#endif
