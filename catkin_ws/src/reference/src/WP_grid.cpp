#include "WP_grid.h"
#include <limits>
#include <cmath>
#include "ros/ros.h"

void intersection_line_segment(const float s1[2], const float s2[2], float m,
  float q, int *is_intersecting, float intersection_point[2])
{																				//TESTED: WORKS
	/*  see --> http://stackoverflow.com/questions/563198/how-do-you-detect-where-two-line-segments-intersect/565282#565282 */
  /*  is_intersecting = 0 --> no collision */
  /*  is_intersecting = 1 --> 1 point collision */
  /*  is_intersecting = 2 --> collinear */
  /*  */
  /*   CAN BE EXTENDED EASILY TO LINE-LINE or SEGMENT-SEGMENT */
  /* initialization */
	float p[2];
	p[0] = 0;
	p[1] = 0;
	float r[2];
	r[0] = 0;
	r[1] = 0;
	intersection_point[0] = 0;
	intersection_point[1] = 0;
	*is_intersecting = 0;  //init to non-intersection
	float q1 [2];
	float s [2];
	float r_cross_s;   //scalar because z component is zero, hence first and second component of cross product are zero
	float q_minus_p_cross_r;   //scalar because z component is zero, hence first and second component of cross product are zero
	float u;
	if ((m != std::numeric_limits<float>::infinity()) && (m != 0)){
		p[0] = -q/m;
		p[1] = 0;
		r[0] = 1;
		r[1] = m;
	} else if (m == std::numeric_limits<float>::infinity()) {
		p[0] = q;
		p[1] = 0;
		r[0] = 0;
		r[1] = 1;
	} else {
		p[0] = 0;
		p[1] = q;
		r[0] = 1;
		r[1] = 0;
	}	
	q1[0] = s1[0];
	q1[1] = s1[1];
	s[0] = s2[0]-s1[0];  //s = s2 - s1
	s[1] = s2[1]-s1[1];
	r_cross_s = r[0]*s[1]-r[1]*s[0];
	q_minus_p_cross_r = (q1[0]-p[0])*r[1]-(q1[1]-p[1])*r[0];
	if (std::abs(r_cross_s)==0 && std::abs(q_minus_p_cross_r)==0){
		//COLLINEAR
		*is_intersecting = 2;
		intersection_point [0] = 0;   //To not be used BTW
		intersection_point [1] = 0;
	} else if (std::abs(r_cross_s)==0 && std::abs(q_minus_p_cross_r)!=0){
		//PARALLEL
		*is_intersecting = 0;
		intersection_point [0] = 0;   //To not be used BTW
		intersection_point [1] = 0;
	} else if (std::abs(r_cross_s)!=0){
		u = q_minus_p_cross_r / r_cross_s;
		if (u>=0 && u<=1){
			//INTERSECTION
			*is_intersecting = 1;
			intersection_point [0] = q1[0]+u*s[0];
			intersection_point [1] = q1[1]+u*s[1];
		}
	}
}


void find_closest_2D(float **points, const float initial_point[2], float closest_point[2], int *index, int N_points){             //TESTED: WORKS
	*index = 0;
	float distance;
	float temp_distance;
	closest_point[0] = points[0][0];
	closest_point[1] = points[0][1];
	distance = sqrt(pow(points[0][0]-initial_point[0],2)+pow(points[0][1]-initial_point[1],2));
	
	for (int i=1; i<N_points; i++){
		temp_distance = sqrt(pow(points[i][0]-initial_point[0],2)+pow(points[i][1]-initial_point[1],2));	
		if (temp_distance < distance){
			distance = temp_distance;
			*index = i;
			closest_point[0] = points[i][0];
			closest_point[1] = points[i][1];
		}
	}
}


void is_convex(float **vertex, bool convexify, bool *convex, int *N_vertex){               //TESTED: WORKS
	float x0[*N_vertex];
	float x1[*N_vertex];
	float x2[*N_vertex];
	float y0[*N_vertex];
	float y1[*N_vertex];
	float y2[*N_vertex];
	float dx1[*N_vertex];
	float dy1[*N_vertex];
	float dx2[*N_vertex];
	float dy2[*N_vertex];
	float zcrossproduct[*N_vertex];
	int t1 = 0;
	int t2 = 0;
	int sum_clock = 0;
	int index_removed = 0;
	bool clockwise = false;
	/*ROS_INFO("N: %d", *N_vertex);
	for (int i=0 ; i< *N_vertex; i++){
		ROS_INFO("Vertex %d: %f - %f", i+1, vertex[i][0], vertex[i][1]);
	}*/

	if (*N_vertex < 3){   //not a polygon
		*convex = false;
		return;
	}
	for (int i=0; i< *N_vertex; i++){
		x0[i] = vertex[i][0];
		y0[i] = vertex[i][1];
		if (i < *N_vertex-2){
			x1[i] = vertex[i+1][0];
			y1[i] = vertex[i+1][1];
			x2[i] = vertex[i+2][0];
			y2[i] = vertex[i+2][1];
		} else if (i == *N_vertex-2){
			x1[i] = vertex[i+1][0];
			y1[i] = vertex[i+1][1];
			x2[i] = vertex[0][0];       //circular, it starts again from zero
			y2[i] = vertex[0][1];
		} else if (i == *N_vertex-1){
			x1[i] = vertex[0][0];
			y1[i] = vertex[0][1];
			x2[i] = vertex[1][0];       //circular
			y2[i] = vertex[1][1];
		}
		dx1[i] = x2[i]-x1[i];
		dy1[i] = y2[i]-y1[i];
		dx2[i] = x0[i]-x1[i];
		dy2[i] = y0[i]-y1[i];
		zcrossproduct[i] = dx1[i]*dy2[i]-dy1[i]*dx2[i];
		if (zcrossproduct[i] >= 0) t1++;
		if (zcrossproduct[i] < 0) t2++;
	}
	*convex = ((t1 == *N_vertex) || (t2 == *N_vertex));
	//ROS_INFO("t1: %d - t2: %d", t1, t2);
	//ROS_INFO("CONVEX? %s", *convex ? "true" : "false");

	if (*convex){
		//ROS_INFO("CONVEX. returning");
		return;
	} else if (convexify){
		//ROS_INFO("Not convex. CONVEXIFYING");
		for (int i=0; i< *N_vertex; i++){
			if (i < *N_vertex-1){
				sum_clock += ((vertex[i][0]-vertex[i+1][0])*(vertex[i][1]+vertex[i+1][1]));
			} else {
				sum_clock += ((vertex[i][0]-vertex[0][0])*(vertex[i][1]+vertex[0][1]));    //circular
			}
		}
		clockwise = sum_clock <= 0;
		//ROS_INFO("Clockwise: %s", clockwise ? "true" : "false");
		for (int i=0; i< *N_vertex; i++){
			if ((clockwise && zcrossproduct[i]>0) || (!clockwise && zcrossproduct[i]<0)){
				for (int j = i-index_removed+1; j<*N_vertex-1-index_removed; j++){
					vertex[j][0] = vertex[j+1][0];
					vertex[j][1] = vertex[j+1][1];
				}
				index_removed++;
			}
		}
		//ROS_INFO("Removed %d", index_removed);
		*N_vertex -= index_removed;      //the new vertices to check are less because some were eliminated by the first convexyfying step
		is_convex(vertex, convexify, convex, N_vertex);   //RECURSIVE UNTIL CONVEX!!
	} else {
		return;         //NOT convex and we don't want to convexify
	}
}


void WP_grid(float **vertex, int *N_vertex, const float initial_position[2], const float d, float **WP, bool *success, int *number_WP){
	//variables
	int index_WP = 0;
	int index_temp_WP = 0;
	float **temp_WP;
	temp_WP = new float *[2];
	for(int i = 0; i<2; i++){
    	temp_WP[i] = new float[2];
	}
	float closest_point [2];
	float initial_pos_internal [2];
	int index = 0;
	float m;
	float q;
	float q_inc;
	bool convex;
	bool positive_intersection = false;
	float s1[2];
	float s2[2];
	int is_intersecting;
	float intersection_point[2];
	bool collision = true;
	const int MAX_WP = 150;     //TODO try to not make it hardcoded
	//initialize
	for (int i=0; i<2; i++){
		for (int j=0; j<2; j++){
			temp_WP [i][j] = 0;
		}
	}
	*success = false;
	*number_WP = 0;
	
	//----START AGLORITHM-----
	ROS_INFO("GRID: starting");
	is_convex(vertex, true, &convex, N_vertex);
	//is_convex(points, true, &convex, &N_vertex);

	if (!convex){
		ROS_INFO("GRID: problem with convex");
		return;     //Some error in convex. Probably less that 3 vertex
	}

	ROS_INFO("GRID: OK convex");

	find_closest_2D(vertex, initial_position, closest_point, &index, *N_vertex);	
	WP[index_WP][0] = closest_point[0];   //filling first waypoint
	WP[index_WP][1] = closest_point[1];
	index_WP++;
	//ROS_INFO("GRID: filling first WP");
	if (index == *N_vertex-1){
		WP[index_WP][0] = vertex[0][0];   //filling second waypoint
		WP[index_WP][1] = vertex[0][1];
		index_WP++;
		//ROS_INFO("GRID: filling second WP (1) Index: %d", index);
	} else {
		WP[index_WP][0] = vertex[index+1][0];   //filling second waypoint
		WP[index_WP][1] = vertex[index+1][1];
		index_WP++;
		//ROS_INFO("GRID: filling second WP (2). Index: %d", index);
	}
	
	if (WP[0][0] != WP[1][0]){     //first and second waypoint has different x
		m = (WP[0][1] - WP[1][1]) / (WP[0][0] - WP[1][0]);
		q = WP[0][1] - m * WP[0][0];
		q_inc = d / sin(M_PI/2-atan(m));
	} else {     //first and second waypoint has same x
		m = std::numeric_limits<float>::infinity();
		q = WP[0][0];
		q_inc = d;
	}
	//ROS_INFO("GRID: m: %f - q: %f - q_inc: %f", m, q, q_inc);

	for (int i=0; i<*N_vertex; i++){
		s1[0] = vertex[i][0];
		s1[1] = vertex[i][1];
		if (i<*N_vertex-1){       //circular problem
			s2[0] = vertex[i+1][0];
			s2[1] = vertex[i+1][1];
		} else {
			s2[0] = vertex[0][0];
			s2[1] = vertex[0][1];
		}
		intersection_line_segment(s1, s2, m, q+q_inc, &is_intersecting, intersection_point);
		//ROS_INFO("GRID: Positive intersection: s1: %f-%f - s2: %f-%f - m: %f - q: %f - int: %d - point: %f-%f",s1[0], s1[1], s2[0], s2[1], m, q, is_intersecting, intersection_point[0], intersection_point[1]);
		if (is_intersecting > 0){     //we have intersection with at least one segment. we can break the for
			positive_intersection = true;
			//ROS_INFO("GRID: Positive intersection");
			break;
		}
	}
	while (collision){
		//ROS_INFO("GRID: inside while");
		if (index_WP>=MAX_WP){          //too much WP
			*number_WP = MAX_WP;
			*success = false;
			ROS_INFO("GRID: too much WP");
			goto end;                  //END
		}
		collision = false;
		if (positive_intersection){
			q += q_inc;
		} else {
			q -= q_inc;
		}
		for (int i=0; i<*N_vertex; i++){
			s1[0] = vertex[i][0];
			s1[1] = vertex[i][1];
			if (i<*N_vertex-1){       //circular problem
				s2[0] = vertex[i+1][0];
				s2[1] = vertex[i+1][1];
			} else {
				s2[0] = vertex[0][0];
				s2[1] = vertex[0][1];
			}
			//ROS_INFO("GRID: testing segment: %f-%f --> %f-%f", s1[0], s1[1], s2[0], s2[1]);
			intersection_line_segment(s1, s2, m, q, &is_intersecting, intersection_point);
			//ROS_INFO("GRID: intersection point: %f-%f", intersection_point[0], intersection_point[1]);
			if (is_intersecting == 1){  //one intersection
				collision = true;
				temp_WP[index_temp_WP][0] = intersection_point[0];
				temp_WP[index_temp_WP][1] = intersection_point[1];
				index_temp_WP++;
				//ROS_INFO("GRID: collision 1");
			} else if (is_intersecting == 2){        //collinear --> the two WP are the edges of the i-th segment (points s1 and s2)
				//ROS_INFO("GRID: collision 2");
				collision = true;
				index_temp_WP = 0;            //absolutely needed to reset the temp_WP found, since the two WP from collinearity already includes prevoius WP found, otherwise out of range
				temp_WP[index_temp_WP][0] = s1[0];
				temp_WP[index_temp_WP][1] = s1[1];
				index_temp_WP++;
				temp_WP[index_temp_WP][0] = s2[0];
				temp_WP[index_temp_WP][1] = s2[1];
				index_temp_WP++;
			} else {
				//ROS_INFO("GRID: no collision");	
			}
			if (index_temp_WP == 2){   //found two temp WP: time to fill real WP
				index_temp_WP = 0;
				initial_pos_internal[0] = WP[index_WP-1][0];  
				initial_pos_internal[1] = WP[index_WP-1][1];
				find_closest_2D(temp_WP, initial_pos_internal, closest_point, &index, 2);
				WP[index_WP][0] = closest_point[0];
				WP[index_WP][1] = closest_point[1];
				index_WP++;
				if (index == 0){
					WP[index_WP][0] = temp_WP[1][0];
					WP[index_WP][1] = temp_WP[1][1];
					index_WP++;
				} else {
					WP[index_WP][0] = temp_WP[0][0];
					WP[index_WP][1] = temp_WP[0][1];
					index_WP++;
				}
			}
		}
	}
	//ROS_INFO("GRID: END");     //if algorythm was succesfull
	*number_WP = index_WP;
	*success = true;
	end:
	delete[] temp_WP;
	//DO NOTHING. Algorythm ended (succesfully or not)*/
}




