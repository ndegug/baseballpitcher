// Online C++ compiler to run C++ program online
#include <iostream>
#include <cstdlib>
#include <ctime>

using namespace std;
class state// game state
{
public:
    
    //scoreboard stats
    int outs=0;
    int balls=0;
    int strikes=0;
    int runs=0;
    
  struct player {//defines properties of a player
    int speed;
    int bat;
    string name;
    int basedesired;
  };
  
//declair players
player p0;
player p1;
player p2;
player p3;
player p4;
player p5;
player p6;
player p7;
player p8;
player p9;



    player base[4];//4 bases, base[0] is "at bat"
    player lineup[9];//lineup with players in order todo: expand to fit full team
    int l=0; //indexes lineup
   // int b; //indexes bases
    state();        //Constructor
    void loadBatter(); //load and declair batter
    void readbases();//to display the bases
    void run(); //for running
    void catches();//catching players or setting their desired bases
    int fate();//random number generator from 1 to 10
    void walk();//for taking the walk
    void inningchange(); //win or change inning
    void defeat(); //enemy runs enough to win
    void pitchbat();//collects pitch and determines outcome of inidvidual swing
    void out(); //send a player out and run innning change if 3

    
    //matrixes
    int batskillmat[3][3][3] = 
    {//master array elements are chance thresholds: {strike/ball, ball/hit, hit/homerun} todo: add digit to chance for home run
        {// ground ball/bad throw
            {40, 80, 90}, //skill 0
            {10, 80, 90}, //skill 1
            {10, 70, 90} //skill 2
        },
        {//fast ball
            {70, 80, 90}, //skill 0
            {30, 60, 90}, //skill 1
            {10, 50, 90} //skill 2
        }, 
        {//curve ball todo: update values
            {70, 80, 90}, //skill 0
            {30, 60, 90}, //skill 1
            {10, 50, 90} //skill 2
        } 
    };
    
    int runskillmat[4][2]=
    {//master array elements are chance thresholds: {out/run one, run one/run two}
        {110,110},//no speed, impossible thresholds for Nobody
        {70,80},//speed 1
        {30,60},//speed 2
        {10,50}//speed 3
    };
    //endmatrixes
    
    //int speedtest=3; //temp number for speed

};


state::state()
{//constructor initialize objects

srand(time(0));//grabs random seed

p0.name="Nobody";//null player for empty bases
p0.speed=0;
p0.bat=0;
p0.basedesired=0;

p1.name="Elsbury";
p1.speed=3;//todo: make this 3 when you know the running system works
p1.bat=1;
p1.basedesired=0;

p2.name = "Pedroia";
p2.speed=2;
p2.bat = 2;
p2.basedesired=0;

p3.name="Ortiz";
p3.speed=1;
p3.bat=3;
p3.basedesired=0;

p4.name= "Napoli";
p4.speed=1;
p4.bat=1;
p4.basedesired=0;

p5.name = "Gomes";
p5.speed=1;
p5.bat=2;
p5.basedesired=0;
	
p6.name= "Victorino";
p6.speed=2;
p6.bat=2;
p6.basedesired=0;
	
p7.name= "Bogarts";
p7.speed=2;
p7.bat=1;
p7.basedesired=0;

p8.name= "Drew";
p8.speed=2;
p8.bat=1;
p8.basedesired=0;

p9.name= "Ross";
p9.speed=1;
p9.bat=2;
p9.basedesired=0;

base[0]=p0;base[1]=p0; base[2]=p0; base[3]=p0;  //bases
lineup[0]=p1;lineup[1]=p2;lineup[2]=p3;lineup[3]=p4; //lineup
lineup[4]=p5;lineup[5]=p6;lineup[6]=p7;lineup[7]=p8;lineup[8]=p9; 
}
int state::fate()
{//random number generator
    
     return (rand() % 100) + 1;
};

void state::pitchbat()//select pitch and determine batter response
{
    int pit;//pitch choice
    int f; //fate
    int a; //strike/ball threshold detrmined by the matrix
    int b; //ball/hit threshold detrmined by the matrix
    int c; //hit/home threshold detrmined by the matrix

    //collect pitch choice
    cout << "Choose your pitch: 0-ground 1-fast 2-curve"<< endl;
    cin >> pit;

    cout << "You chose: "<< pit << endl;//todo: use dictionary to print name of pitch

    //replace invalid answers with 0
    switch (pit) {
  case 0:
    break;
  case 1:
    break;
  case 2:
    break;
  default:
    cout << "Your pitcher didn't understand what you said and made a bad throw."<< endl;
    pit=0;
}
    a=batskillmat[pit][base[0].bat][0];//load thresholds
    b=batskillmat[pit][base[0].bat][1];
    c=batskillmat[pit][base[0].bat][2];


    //determine fate and compare to probability matrix
    f=fate();

    //conditional statement determining strike/ball/hit/hr based on fate
    if (f<=a)
    {//strike
    cout<< "Strike"<< endl;
    strikes++;
      if (strikes==3)//if three strikes, out
      {
       cout<< base[0].name<<" strikes out!"<< endl;
       outs++;
        if (outs==3){//check for three outs
          inningchange();//should either end the game or reset the inning
        }
        else
        {
          loadBatter();
        };
       balls=0; strikes=0; //reset swing outcomes
      }
    }
    else if (f>a && f<=b)
    {//ball
      cout<< "ball"<< endl;
      balls++;
      if (balls==4)//if four balls, take the walk
      {
        walk();
        balls=0; strikes=0; //reset swing outcomes
      }
    }
    else if (f>b && f<=c)
    {//hit
      cout<< "It's a hit!"<< endl;
      run();
    }
    else
    {//home run
        cout<< "It's gone!"<< endl;
        for (int i = 2; i >= 0; i--) {
            if (base[i].name != "Nobody")//add a run for every real player
            {
                runs++;//add run
                base[i]=p0;//empty base
            };
      
        };
        
    };

};
void state::inningchange()//reset game if tied or win game todo: move contents to condition in method "outs"
{
  if (runs==1){//tie, reset game
    cout<< "You tied, but they got another run at the bottom of the inning, new inning!"<<endl;
    runs=0; outs=0; balls=0; strikes=0;//resets stats
    base[0]=p0;base[1]=p0;base[2]=p0;base[3]=p0;//empty bases
  }
  else{
     cout<< "You win!"<<endl;
     exit(0);
  };
};


void state::defeat()
{
    cout << "You lose"<<endl;
    exit(0);
};

void state::loadBatter()
{//load batter
//cout<<l<<endl;
base[0]=lineup[l];
l++;
    if (l>8){//reset lineup index
    l=0;
    };
};

void state::readbases()
{//displays the bases
    cout <<"Current field:"<< endl;
    cout << base[0].name<<", "<< base[1].name<<", "<< base[2].name<<", "<< base[3].name<< endl;
};

void state::catches()//determines who gets caught and eliminated from the field, if they don't get caught, set a basedesired
{
    int f;
    int a;
    int b;
for (int i = 3; i >= 0; i--) {
    f=fate();
 // cout<< "fate has decided: "<< f<< endl;
  a=runskillmat[base[i].speed][0]; //threshold 1
  b=runskillmat[base[i].speed][1]; //threshold 2
// cout<< "Speed: "<<base[i].speed<<" threshes: " << a<<","<<b<<endl;
  //conditional statements for caught, single and double. todo: incorperate skill
  if (f<=a && base[i].name!="Nobody"){//caught, remove and add an out
      cout<< base[i].name<<" is caught, he's out!"<< endl;
      base[i]=p0; //empty the base of the caught player
      outs++;
      if (outs==3){//check for three outs
        inningchange();//should either end the game or reset the inning
        break;//break the base searching loop
      };
  }
  else if (base[i].name=="Nobody"){
      NULL;
  }
  else if(f>a && f<=b){//goal is one base ahead
      cout<< base[i].name<<" wants to run one base"<< endl;
      base[i].basedesired=1+i;
  }
  else{ //goal is 2 bases ahead
      cout<< base[i].name<<" wants to run two bases"<< endl;
      base[i].basedesired=2+i;
  };

    };
};

void state::run()//makes players run bases
{
    int x;
    catches();
for (int i = 3; i >= 0; i--) {
  
  x=base[i].basedesired;//each player intended base
//  x=base[i].speed+i;//todo: update to incorperate basedesired attribute
  if (x>=4){//todo: incorperate luck
      cout<< base[i].name << " makes it home!"<< endl;
      runs++;
      if (runs==2){
        defeat();
      };
      base[i]=p0;
  }//todo: add condition where base intended is occupied
  else if (base[x].name!="Nobody"){//base occupied
      while (base[x].name!="Nobody"){//search backwards to find unoccupied base
          x--;
      }
      base[x]=base[i];//take it
  }
  else{
      base[x]=base[i];
     // cout << base[i].name << " moves to base "<< x << endl;
      base[i]=p0;
      //cout << base[i].name << " is on base "<< i << " now"<< endl;
  }
}
//base[0+speedtest]=base[0];//one instance of a player running bases todo: import for loop from old code
base[0]=p0;
};

void state::walk()//make players take the walk
{
    cout<<base[0].name << " takes the walk."<<endl;
    if (base[3].name!="Nobody"){
        cout<<base[3].name<< " walks home!"<<endl;
            runs++;
            if (runs==2){
            defeat();
             };
    };
   for (int i = 2; i >= 0; i--) {
      base[i+1]=base[i];
   };
};

int main() {
state field;//creates the field
int i;
for (i = 0; i <2; i++) {//todo: change to a while loop to referance a "game on" int within the class instance
field.pitchbat();
//field.loadBatter();
//field.readbases();
//field.run();
//field.loadBatter();
//field.readbases();
//field.walk();
}
    return 0;
}