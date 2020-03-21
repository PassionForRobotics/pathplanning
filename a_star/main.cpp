#include <bits/stdc++.h> 
#include <fstream>


#define TRY_BIG_RES (false)

#if (TRY_BIG_RES == true)
    #include "image.h"
#else
    #include "image2.h"
#endif




#define TEST_MODE (false)
#define TEST_1 (true && TEST_MODE)

using namespace std; 

#if (TEST_MODE==true)
    
    #define GRID_SIZE_X   (5)
    #define GRID_SIZE_Y   (5)

#else
    
    #define GRID_SIZE_X  (IMAGE_WIDTH) //(5)
    #define GRID_SIZE_Y  (IMAGE_HEIGHT) //(5)

#endif

// Origin is at left top


struct Cell{
    int x, y; 
    float f;
};
typedef struct Cell Cell;


Cell path[GRID_SIZE_X][GRID_SIZE_Y]; // output map
Cell cost_map[GRID_SIZE_X][GRID_SIZE_Y]; // output map

Cell grid[GRID_SIZE_X][GRID_SIZE_Y] 
#if (TEST_MODE==true)
    #if (TEST_1==true)
        = { 
            {{.x=0, .y=0, .f=1.0f},{.x=1, .y=0, .f=0.0f},{.x=2, .y=0, .f=0.0f},{.x=3, .y=0, .f=1.0f},{.x=4, .y=0, .f=1.0f}},
            {{.x=0, .y=1, .f=1.0f},{.x=1, .y=1, .f=0.0f},{.x=2, .y=1, .f=1.0f},{.x=3, .y=1, .f=1.0f},{.x=4, .y=1, .f=1.0f}},
            {{.x=0, .y=2, .f=1.0f},{.x=1, .y=2, .f=0.0f},{.x=2, .y=2, .f=1.0f},{.x=3, .y=2, .f=1.0f},{.x=4, .y=2, .f=1.0f}},
            {{.x=0, .y=3, .f=1.0f},{.x=1, .y=3, .f=0.0f},{.x=2, .y=3, .f=1.0f},{.x=3, .y=3, .f=1.0f},{.x=4, .y=3, .f=1.0f}},
            {{.x=0, .y=4, .f=1.0f},{.x=1, .y=4, .f=1.0f},{.x=2, .y=4, .f=1.0f},{.x=3, .y=4, .f=1.0f},{.x=4, .y=4, .f=1.0f}},
             
        };
    #else
        ;
    #endif
#else

    ;    // From image

#endif
//map<int, string, greater <int> > mymap; 
//map<pair<int, int>, float, greater<float>> not_explored;
multimap<float, pair<int, int>, less<float>> not_explored;
multimap<float, pair<int, int>> explored;

Cell start_pos;
Cell goal_pos;
Cell current_pos;

float cost_depth ;

void printCells(Cell cells[GRID_SIZE_X][GRID_SIZE_Y], int w , int h);
bool findMultiMapByValue(multimap<float, pair<int, int> > mp , pair<int, int> value, multimap<float, pair<int, int> >::iterator *ret_it);
void initializeCells(Cell cells[GRID_SIZE_X][GRID_SIZE_Y], int w , int h){
    //cout << "Array :" << aname << " : ";
    for(auto i = 0 ; i < w ; i++){
        for(auto j = 0 ; j < h ; j++){
             
             #if (TEST_MODE==true)
                
                 cells[i][j].x = i ;
                 cells[i][j].y = j ; 
                 
                 #if (TEST_1==true)
                     //
                 #else    
                    cells[i][j].f = 1.0f;
                 #endif
                 path[i][j].f = 0.0f; // This can become bug later
                 cost_map[i][j].f = 0.0f; // This can become bug later
                 
             #else
                cells[i][j].f = (float)(image[i][j]>0?1.0f:0.0f);
             #endif
             //cells[i][j].g = 0.0f;
             //cells[i][j].h = 0.0f;
             //cells[i][j].hasExplored = false;
        } 
    }
    
    #if (TEST_MODE==true)
    cout << "\ngrid \n";
    printCells(cells, w, h);
    cout << "\npath \n";
    printCells(path, w, h);
    cout << "\ncost map \n";
    printCells(cost_map, w, h);
    
    #endif
}

  
void printCells(Cell cells[GRID_SIZE_X][GRID_SIZE_Y], int w , int h){
    //cout << "Array :" << aname << " : ";
    for(auto i = 0 ; i < w ; i++){
        for(auto j = 0 ; j < h ; j++){
            cout << " "
            //[" <<cells[i][j].x << ", "<<cells[i][j].y << "] "
             << setfill('0') << setw(6) 
          << fixed << setprecision(3) << cells[i][j].f << " ";
        }
        cout << "\n";
    }
}

//setfill('0') << setw(5) 
//          << fixed << setprecision(2) << it->first

void printMultiMap(string name, multimap<float, pair<int, int> > mp){

    cout << name << "\n";
    for(multimap<float, pair<int, int>>::const_iterator it = mp.begin();
        it != mp.end(); ++it)
    {
        std::cout << "[" << it->second.first << " " << it->second.second << "] " 
        << setfill('0') << setw(6) 
        << fixed << setprecision(3) << it->first << "\n";
    }
    cout  << "\n";
}
void printMap(string name, map<float, pair<int, int> > mp){

    cout << name << "\n";
    for(map<float, pair<int, int>>::const_iterator it = mp.begin();
        it != mp.end(); ++it)
    {
        std::cout << "[" << it->second.first << " " << it->second.second << "] " 
        << setfill('0') << setw(6) 
        << fixed << setprecision(3) << it->first << "\n";
    }
    cout  << "\n";
}

//void printVector(vector<pair<int, int>, float> vec){
//
//    for(vector<pair<int, int>, float>::const_iterator it = vec.begin();
//        it != vec.end(); ++it)
//    {
//        std::cout << "[" << it->first.first << " " << it->first.second << "] " << it->second << "\n";
//    }
// 
//}

bool validateMove(Cell pos){
    if( (pos.x>=0 && pos.x<GRID_SIZE_X)
     && 
     (pos.y>=0 && pos.y<GRID_SIZE_Y)){
        return true;
     }   
     return false;
}

// Verified
void validMoves(Cell moves[3][3], Cell *pos){
    
    //Cell moves[3][3];
    
    for(auto i = 0 ; i < 3 ; i++){
        for(auto j = 0 ; j < 3 ; j++){
             moves[i][j].x = i+pos->x - 1 ;
             moves[i][j].y = j+pos->y - 1 ; 
             
             if(
             (true == validateMove(moves[i][j])
             &&
             (grid[moves[i][j].x][moves[i][j].y].f==1.0f) // Movable area
             )
             ) 
             {
                moves[i][j].f = 1.0f;  // It can be used to check if move is valid
             }        
                               
             //moves[i][j].g = 0.0f;
             //moves[i][j].h = 0.0f;
             //moves[i][j].hasExplored = false;
             
//             cout << "{["<<moves[i][j].x<<","<<moves[i][j].y<<"] " 
//             << i+pos->x - 1  << ", " << i+pos->y - 1 << " | "
//             << moves[i][j].f << "} ";
        } 
        //cout << "\n";
    }

}

float costFunction(Cell pos){
    
    //float distance_from_start = sqrt((start_pos.x-pos.x)*(start_pos.x-pos.x) + (start_pos.y-pos.y)*(start_pos.y-pos.y));
    float distance_to_goal = sqrt((goal_pos.x-pos.x)*(goal_pos.x-pos.x) + (goal_pos.y-pos.y)*(goal_pos.y-pos.y));
    
    return round((/*distance_from_start+*/distance_to_goal)*10000.0f)/10000.0f;
}

//
//bool sortByVal(const pair<pair<int, int>, float> &a, 
//               const pair<pair<int, int>, float> &b) 
//{ 
//    return (a.second < b.second); 
//} 
//
//vector<pair<int, int>, float> sortMapV(map<pair<int, int>, float> exploration){
//    // create a empty vector of pairs
//    
//    vector<pair<int, int>, float> vec;
//
//	// copy key-value pairs from the map to the vector
//    map<pair<int, int>, float> :: iterator it2;
//    for (it2=exploration.begin(); it2!=exploration.end(); it2++) {
//        vec.push_back(make_pair(it2->first, it2->second));
//    }
//    
//    sort(vec.begin(), vec.end(), sortByVal); 
//
//}
//
//
//set<pair<pair<int,int>, float> sortMapS(map<pair<int, int>, float> exploration){
//    // Declaring the type of Predicate that accepts 2 pairs and return a bool
//	typedef std::function<bool(pair<pair<int,int>, float>, pair<pair<int,int>, float>)> Comparator;
// 
//	// Defining a lambda function to compare two pairs. It will compare two pairs using second field
//	Comparator compFunctor =
//			[](pair<pair<int,int>, float> elem1, pair<pair<int,int>, float> elem2)
//			{
//				return elem1.second < elem2.second;
//			};
// 
//	// Declaring a set that will store the pairs using above comparision logic
//	set<pair<pair<int,int>, float>, Comparator> setOfWords(
//			exploration.begin(), exploration.end(), compFunctor);
// 
//}

bool hasReachedGoal(Cell pos){
    
    if((pos.x==goal_pos.x)&&(pos.y==goal_pos.y)){
        // set final settings to the maps
        
        
        #if (TEST_MODE==true)
        printMultiMap(string("not_explored"), not_explored);   
        #endif        
        
        auto it = not_explored.begin();
 
        explored.insert(make_pair(it->first, make_pair(pos.x, pos.y)));
        path[pos.y][pos.x].f = 1.0;
        
        #if (TEST_MODE==true)
        printMultiMap(string("explored"), explored); 
        #endif
        
        multimap<float, pair<int, int> >::iterator ret_it;

        // remove the move from not_explored
        if(true==findMultiMapByValue(not_explored, make_pair(it->second.first, it->second.second), &ret_it)){
            not_explored.erase(it);
        }
        return true;
    }
    
    return false;
}

bool findMultiMapByValue(multimap<float, pair<int, int> > mp , pair<int, int> value, multimap<float, pair<int, int> >::iterator *ret_it=NULL){

    bool result = false;
    
    for (auto it = mp.begin(); it != mp.end(); ++it) {
        if((it->second.first == value.first)
            &&
          (it->second.second == value.second) )
		{
			// Yes found
			result = true;
            if(ret_it!=NULL){
                * ret_it = it;
            } 
            // It can be broken here as well.
        }
        
    }
    return result;
}
bool findMapByValue(map<float, pair<int, int> > mp , pair<int, int> value, map<float, pair<int, int> >::iterator *ret_it=NULL){
    
    bool result = false;
    
    for (auto it = mp.begin(); it != mp.end(); ++it) {
        if((it->second.first == value.first)
            &&
          (it->second.second == value.second) )
		{
			// Yes found
			result = true;
            if(ret_it!=NULL){
                * ret_it = it;
            } 
            // It can be broken here as well.
        }
        
    }
        
        
//    auto it = mp.begin();
//	// Iterate through the map
//	while(it != mp.end())
//	{
//		// Check if value of this entry matches with given value
//		if((it->second.first == value.first)
//            &&
//          (it->second.second == value.second) )
//		{
//			// Yes found
//			result = true;
//            // It can be broken here as well.
//        }
//    }
    
    return result;
}

bool exploration(){
    // check for valid moves and put it in to_be_explored array
    Cell moves[3][3];
    
    // Check whether reached goal
    // Termination criterial
    if(hasReachedGoal(current_pos)==true){
        return true;
    }else{
        
        
        validMoves(moves, &current_pos);
        
        for(auto i = 0 ; i < 3 ; i++){
            for(auto j = 0 ; j < 3 ; j++){
                
                // Also check if the move[i][j] is not in all the maps
                // not_explored
                // explored
                // costmap
                
                // find
                
                if( 
                (findMultiMapByValue(not_explored, make_pair(moves[i][j].x, moves[i][j].y)) == false)
                &&    
                (findMultiMapByValue(explored, make_pair(moves[i][j].x, moves[i][j].y)) == false)
                &&
                (!( (moves[i][j].x == start_pos.x) && (moves[i][j].y == start_pos.y) ))
                )
                {   
                    if((moves[i][j].f==1.0f) 
                    && (grid[moves[i][j].x][moves[i][j].y].f==1.0f)
                    ){
                        //if( (moves[i][j].x == current_pos.x) && (moves[i][j].y == current_pos.y) ){
                            // Nothing 
                        //}else{
                            not_explored.insert(make_pair(1.0+costFunction(moves[i][j]), make_pair(moves[i][j].x, moves[i][j].y)));
                            cost_map[moves[i][j].x][moves[i][j].y].f = cost_depth+1.0+costFunction(moves[i][j]);
                        //}
                    }
                }else{
                    // Do not work on the move
                }
            }
        }
        
        #if (TEST_MODE==true)
        printMultiMap(string("not_explored"), not_explored);   
        #endif        
        
        auto it = not_explored.begin();

        current_pos.x = it->second.first;
        current_pos.y = it->second.second;
        
        cost_depth = it->first  - costFunction(current_pos);
        
        explored.insert(make_pair(it->first, make_pair(it->second.first, it->second.second)));
        path[it->second.first][it->second.second].f = 1.0;
        
        #if (TEST_MODE==true)
        printMultiMap(string("explored"), explored); 
        #endif
        
        multimap<float, pair<int, int> >::iterator ret_it;

        // remove the move from not_explored
        if(true==findMultiMapByValue(not_explored, make_pair(it->second.first, it->second.second), &ret_it)){
            not_explored.erase(it);
        }
        
        //getchar();
         
        return false; 
            
    }
    
    return false; 
}

void visulization(){
    
//    for (auto it = explored.begin(); it != explored.end(); ++it) {
//        if(it->second.first){}
//            grid[it->second.first][it->second.second].x = it->second.first;
//            grid[it->second.first][it->second.second].y = it->second.second;
//            grid[it->second.first][it->second.second].f = 1.0; //it->first;
//    }
    
    #if (TEST_MODE==true)
        cout << "\npath\n";
        printCells(path, GRID_SIZE_X, GRID_SIZE_Y);
        cout << "\ncost_map\n";
        printCells(cost_map, GRID_SIZE_X, GRID_SIZE_Y);
        
    #endif
}

void pathUpdate(){
    // sort the exploration (to_be_explored) and// add the best to the explored and update the path
}

// Whether reached to the goal and rest of the updates
void checkStatus(){

}


void saveImage(Cell cells[GRID_SIZE_X][GRID_SIZE_X], int w, int h, string file_name){
    
    stringstream ss;
    string image_save;
    
    //P3 2 3 255
    image_save.append("P1 ");
    ss << w;
    image_save.append(ss.str());
    ss.str(std::string());
    
    image_save.append(" ");
    ss.str(std::string());
    
    ss << h;
    image_save.append(ss.str());
    ss.str(std::string());
    
    image_save.append(" ");
    ss.str(std::string());
    
    ss << 0;
    image_save.append(ss.str());
    ss.str(std::string());
    
    image_save.append("\n");
    ss.str(std::string());
    
    long iteration_limit = w*h+1;
    cout << "Saving ...";
    for(auto i = 0 ; i < w ; i++){
        for(auto j = 0 ; j < h ; j++){
            ss << (cells[i][j].f==0.0f?1:0); // As image is inverting
            image_save.append(ss.str());
            ss.str(std::string());
            --iteration_limit;
            
            if(0==iteration_limit%9000){
                cout << ".";
            }
            
        }
    }
    
    
    std::ofstream out(file_name);
    out << image_save;
    out.close();    
    

}

int main() 
{ 
    
    #if (TEST_MODE==true)
        
    start_pos.x = 0;
    start_pos.y = 0;
    start_pos.f = 9999.9;
    

    goal_pos.x = 4;//2008;    
    goal_pos.y = 4;//984;
    


    #else
    
        #if (TRY_BIG_RES == true)
    
    start_pos.x = 1450;//254;//67; //1450;
    start_pos.y = 1272; //67;//254; //1272;
    start_pos.f = 9999.9;
    

    goal_pos.x = 980; //97; //173;//208; //311;// 2012; //1664; //  984;
    goal_pos.y = 2012; //311;//324;// 277; //97;//980; //1595; // 2008;
    
        #else

    start_pos.x = 254;//67; //1450;
    start_pos.y = 67;//254; //1272;
    start_pos.f = 9999.9;
    

    goal_pos.x = 97; //173;//208; //311;// 2012; //1664; //  984;
    goal_pos.y = 311;//324;// 277; //97;//980; //1595; // 2008;
    
        #endif
    
    #endif
    
    cost_depth = 0.0f;

    current_pos = start_pos;

    printf("A star code practice.\n\n");
    
    //      printCells(grid, GRID_SIZE_X, GRID_SIZE_Y);
    initializeCells(grid, GRID_SIZE_X, GRID_SIZE_Y);
    //      printCells(grid, GRID_SIZE_X, GRID_SIZE_Y);
    
    
    saveImage( grid, GRID_SIZE_X, GRID_SIZE_Y, "input.pbm");


    cout << "Processing ...";
    long iteration_limit = GRID_SIZE_X*GRID_SIZE_Y+1;
    while(exploration()==false){
        if(0 == --iteration_limit){
            break;
        }
    }
    cout << "\nDone";
          // minSteps(m, n, d)); 
          
    visulization(); 


    saveImage( path, GRID_SIZE_X, GRID_SIZE_Y, "output.pbm");

//    stringstream ss;
//    
//    //P3 2 3 255
//    image_save.append("P1 ");
//    ss << GRID_SIZE_X;
//    image_save.append(ss.str());
//    ss.str(std::string());
//    
//    image_save.append(" ");
//    ss.str(std::string());
//    
//    ss << GRID_SIZE_Y;
//    image_save.append(ss.str());
//    ss.str(std::string());
//    
//    image_save.append(" ");
//    ss.str(std::string());
//    
//    ss << 1;
//    image_save.append(ss.str());
//    ss.str(std::string());
//    
//    image_save.append("\n");
//    ss.str(std::string());
//    
//    iteration_limit = GRID_SIZE_X*GRID_SIZE_Y+1;
//    cout << "Saving ...";
//    for(auto i = 0 ; i < GRID_SIZE_X ; i++){
//        for(auto j = 0 ; j < GRID_SIZE_Y ; j++){
//            ss << path[i][j].f;
//            image_save.append(ss.str());
//            ss.str(std::string());
//            --iteration_limit;
//            
//            if(0==iteration_limit%9000){
//                cout << ".";
//            }
//            
//        }
//    }
//    
//    
//    std::ofstream out("output.pbm");
//    out << image_save;
//    out.close();
    
    cout << "\nDone";
    
    
    
    return 0; 
} 