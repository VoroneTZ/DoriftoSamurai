

var cuberesolution=128; // set up the resolution of the cube here
var west[3]={180,0,0};
var north[3]={90,0,0};
var east[3]={0,0,0};
var south[3]={-90,0,0};
var down[3]={90,-90,0};
var up[3]={90,90,0};
var FCc=0;


int nv=2;//view level global encounter 
int FDestroyMe=1; //set 0 to free mem

function mtl_Dynenvmap_init()
{
	FCc = FCc+1;
	nv=nv+1;
	int lnv;
	lnv=nv;
	my.skill14=nv;
	
	VECTOR* ppos;
	VIEW* cv_w;
	BMAP* lbm;
	lbm = NULL;
	cv_w =NULL;
	lbm = bmap_createblack(cuberesolution*6,cuberesolution,24);
	bmap_to_cubemap(lbm);
	
	cv_w=view_create(nv);
	
	cv_w.size_x=cuberesolution;
	cv_w.size_y=cuberesolution;
	cv_w.aspect = 1;
	cv_w.arc=90;
	set(cv_w,SHOW);
	
	
	cv_w.genius =me;
	
	ppos = vector (my.x,my.y,my.z+10);
	vec_set(cv_w.x,ppos);
	vec_set(cv_w.pan,west);	
	
	cv_w.bmap = lbm;
	
	
	mtl.skin1 = lbm;	
	
	mat_set(mtl.matrix,matViewInv);
	mtl.matrix41 = 0;
	mtl.matrix42 = 0;
	mtl.matrix43 = 0;	
	while (my.skill20>0)
	{
		
		if (!(my.eflags&CLIPPED))
		{
			ppos = vector (my.x,my.y,my.z+10);
			
			vec_set(cv_w.x,ppos);
			vec_set(cv_w.pan,west);
			cv_w.pos_x = 0;
			

			wait(3);
			cv_w.pos_x = cuberesolution;
			vec_set(cv_w.pan,north);

			wait(3);
			cv_w.pos_x = cuberesolution*2;
			vec_set(cv_w.pan,east);

			wait(3);
			cv_w.pos_x = cuberesolution*3;
			vec_set(cv_w.pan,south);
			

			wait(3);
			cv_w.pos_x = cuberesolution*4;
			vec_set(cv_w.pan,down);

			wait(3);
			cv_w.pos_x = cuberesolution*5;
			vec_set(cv_w.pan,up);
			wait(3);
		}
		else
		wait(5);
	}
	ptr_remove (cv_w);
	ptr_remove (lbm);
	FCc = FCc-1;
}


function mtl_Stenvmap_init()
{	
	nv=nv+1;
	int lnv;
	lnv=nv;
	my.skill14=nv;
	
	VECTOR* ppos;
	VIEW* cv_w;
	BMAP* lbm;
	lbm = NULL;
	cv_w =NULL;
	lbm = bmap_createblack(cuberesolution*6,cuberesolution,24);
	bmap_to_cubemap(lbm);
	
	cv_w=view_create(nv);
	
	cv_w.size_x=cuberesolution;
	cv_w.size_y=cuberesolution;
	cv_w.aspect = 1;
	cv_w.arc=90;
	set(cv_w,SHOW);
	
	
	cv_w.genius =me;
	
	ppos = vector (my.x,my.y,my.z+10);
	vec_set(cv_w.x,ppos);
	vec_set(cv_w.pan,west);	
	
	cv_w.bmap = lbm;
	
	mtl.skin1 = lbm;	

	
	//	mat_set(mtl.matrix,matViewInv);
	//	mtl.matrix41 = 0;
	//	mtl.matrix42 = 0;
	//	mtl.matrix43 = 0;	
	
	ppos = vector (my.x,my.y,my.z+10);
	
	vec_set(cv_w.x,ppos);
	vec_set(cv_w.pan,west);
	cv_w.pos_x = 0;
	

	wait(1);
	cv_w.pos_x = cuberesolution;
	vec_set(cv_w.pan,north);

	wait(1);
	cv_w.pos_x = cuberesolution*2;
	vec_set(cv_w.pan,east);

	wait(1);
	cv_w.pos_x = cuberesolution*3;
	vec_set(cv_w.pan,south);
	

	wait(1);
	cv_w.pos_x = cuberesolution*4;
	vec_set(cv_w.pan,down);
	

	wait(1);
	cv_w.pos_x = cuberesolution*5;
	vec_set(cv_w.pan,up);
	wait(1);

	
	ptr_remove(cv_w);
}


function AssignEnvMap(ENTITY* AEnt)
{
	MATERIAL* l_DynEnvMap;
	AEnt.skill20=1;
	l_DynEnvMap = mtl_create();
	l_DynEnvMap.skill20=1;
	l_DynEnvMap.event = mtl_Dynenvmap_init;	
	l_DynEnvMap.effect = "envmirror.fx";
	AEnt.material = l_DynEnvMap;
	while (AEnt.skill20>0)
	{wait(1);}
	l_DynEnvMap.skill20=0;
	wait(60);
	ptr_remove(l_DynEnvMap);

}


MATERIAL* F_SEnvMap =
{
	event = mtl_Stenvmap_init;	
	effect = "envmirror.fx";
}


function AssignStaticEnvMap(ENTITY* AEnt)
{
	MATERIAL* l_SEnvMap;
	l_SEnvMap = mtl_create();
	l_SEnvMap.event = mtl_Stenvmap_init;	
	l_SEnvMap.effect = "envmirror.fx";
	AEnt.material = l_SEnvMap;
	while (FDestroyMe=1)
	{wait(1);}
	ptr_remove(l_SEnvMap);
}

action mat_S_EnvMap()
{
	AssignStaticEnvMap(me);	
}

action mat_D_EnvMap()
{
	AssignEnvMap(me);	
}


