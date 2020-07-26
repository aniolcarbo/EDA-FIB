#include "Player.hh"


/**
 * Write the name of your player and save this file
 * with the same name and .cc extension.
 */
#define PLAYER_NAME FazeXiui


struct PLAYER_NAME : public Player {

  /**
   * Factory: returns a new instance of this class.
   * Do not modify this function.
   */
  static Player* factory () {
    return new PLAYER_NAME;
  }

  /**
   * Types and attributes for your player can be defined here.
   */


  typedef vector<int> VI;
  typedef vector<VI> Matrix;

  void move_dwarves() {

    int f = 60;
    int c = 60;
    Matrix m2 (f, VI(c,-2));

    //Matrix m3 (f, VI(c,-2));

    distancies_tresors(m2);
    //distancies_enemics(m3);

    VI D = dwarves(me()); //vector amb els id de tots els teus dwarves
    int mida = D.size();
    for(int i=0;i<mida;i++){
      
     
      int id = D[i];
      Pos p = unit(id).pos;
    
      bool voltant = adjacents(p, id);
      
      /*
      //estrategia: anar a per enemics
      if (voltant==false){

        if (p.i==0 or p.j==0 or p.i==59 or p.j==59){
          
          enterMoria(p);
        }
        else{
          pair<int,int> millor_opcio(60,0);
          for (int j = 0;j<8;j++){//mirem al voltant del dwarf en questio
            Pos aux = p + Dir(j);
            if (pos_ok(aux)){
              if (m3[aux.i][aux.j] >= 0 and m3[aux.i][aux.j] < millor_opcio.first){
              millor_opcio.first = m3[aux.i][aux.j];
              millor_opcio.second = j;
              }
              //if (cell(aux).type == 1) pintar = j;
            }
          }
         command((id), Dir(millor_opcio.second)); 
       }

      }*/

      //estrategia: anar a per tresors
      if (voltant==false){ //si no hem de mourens per cap event adjacent(tresor,atacar enemic, fugir)
       
        if (p.i==0 or p.j==0 or p.i==59 or p.j==59){
          
          enterMoria(p);
        }
        else{



          //si no hem d'escapar del balrog:


          //anar a pintar o a per tresors
          pair<int,int> millor_opcio(60,0);
          for (int j = 0;j<8;j++){//mirem al voltant del dwarf en questio
            Pos aux = p + Dir(j);
            if (pos_ok(aux)){
              if (m2[aux.i][aux.j] >= 0 and m2[aux.i][aux.j] < millor_opcio.first){
              millor_opcio.first = m2[aux.i][aux.j];
              millor_opcio.second = j;
              }
             // if (cell(aux).type == 1) pintar = j;
            }
          }
         command((id), Dir(millor_opcio.second)); //si tenim un tresor a menys de 20 anem
          
        }
        
      }


    }
    
  }

  void move_wizards() { 

    int f = 60;
    int c = 60;
    Matrix m1 (f, VI(c,-2));

    distancies_nans(m1);

    VI W = wizards(me()); //guarda tots els wizards en un vector
    int mida = W.size();
    for (int i=0;i<mida;i++) { //amb un for recorrent el vector, decidim que fa cada wizard
      int id = W[i];
      Pos p = unit(id).pos;
      
      //mirar si estan tots els casos contemplats, molts cops es queden quiets
      
      bool escapat = fugir(p);
      //si no tenim tropes enemigues a prop, anem a una casella valida (no roca,granit,abyss)
      if(escapat == false){
        if (p.i==0 or p.j==0 or p.i==59 or p.j==59) enterMoria(p);
        else{
          pair<int,int> millor_opcio(60,0);
          for (int i = 0;i<8 and i%2==0;i++){
            Pos aux = p + Dir(i);
            if (m1[aux.i][aux.j] >= 0 and m1[aux.i][aux.j] < millor_opcio.first){
              millor_opcio.first = m1[aux.i][aux.j];
              millor_opcio.second = i;
            }
          }
          command((cell(p).id), Dir(millor_opcio.second));
        }
      }
   }
  
  }




  ////////////////////////////EXTRES///////////////////////////////

  void enterMoria(Pos p){
    if(p.i==0) command (cell(p).id, Bottom);
    else if(p.j==0) command (cell(p).id, Right);
    else if(p.i==59) command (cell(p).id, Top);
    else if(p.j==59) command (cell(p).id, Left);
  }

  bool adjacents(Pos p, int id){
    for (int z=0;z<8;z++){
      Pos aux = p + Dir(z);
      if (pos_ok(aux)){
        int num = cell(aux).id;
        if (num != -1){
          Unit unitat = unit(num);
          if (cell(aux).treasure){ //hi ha un tresor, l'agafem
           command(id, Dir(z));
           return true;
          }
          else  if (unitat.type == 0 and unitat.player != me()){//hi ha un dwarf enemic, ataquem
            /*if (unitat.health > unit(id).health){ //si tenim mes vida ataquem, altrament fugim
              if (z<=3){
               command(id, Dir(z+4));
               return true;
              }
              else {
                command(id, Dir(z-4));
                return true;
              }
            }
            else{*/
              command(id, Dir(z));
              return true;
            //}
          }
          else  if (unitat.type == 1 and unitat.player != me()){//hi ha un mag enemic, ataquem
            command(id, Dir(z));
            return true;
          }
          else  if (unitat.type == 2){//hi ha un orc, ataquem
            command(id, Dir(z));
            return true;
          }
          else if (unitat.type == 3){//hi ha un troll, escapem
            if (z<=3){
             command(id, Dir(z+4));
             return true;
            }
            else {
              command(id, Dir(z-4));
              return true;
            }
          }
        }
    }
  }
  return false;
}

  
  bool fugir(Pos p){//com tenir en compte diagonals? fer casos per cada i, i comprovar si es valida
    for (int i=0;i<8;i++){
      Pos aux = p + Dir(i);
      if(pos_ok(aux)){
        int n = cell(aux).id;
        if(n != -1){ 
            if(unit(n).type == 2 or unit(n).type == 3){//es orc o troll
              if(i==0 or i==1 or i==7){
                  command(cell(p).id, Top);
                  return true;
                }
                else if(i==3 or i==4 or i==5){
                  command(cell(p).id, Bottom);
                  return true;
                }
                else{
                  command(cell(p).id, Dir(i+4));
                  return true;
                }
            }
            else if (unit(n).type == 0){//es un dwarf enemic
              if ((unit(n).player) != me()){
                if(i==0 or i==1 or i==7){
                  command(cell(p).id, Top);
                  return true;
                }
                else if(i==3 or i==4 or i==5){
                  command(cell(p).id, Bottom);
                  return true;
                }
                else{
                  command(cell(p).id, Dir(i+4));
                  return true;
                }
              }
            }
          }
      }
    }
  return false;
  }

  void distancies_nans(Matrix& m){
    VI nans = dwarves(me());
    queue<Pos> posicions;
    
    for (int i = 0;i<nans.size();i++){ //omplim la matriu amb 0 on dwarf i ens guardem les posicions en un vector
      Pos p_nan = unit(nans[i]).pos;
      posicions.push(p_nan);
      m[p_nan.i][p_nan.j] = 0;
    }

    while (not posicions.empty()){ //recorrem la cua de posicions (si afegim valors amb pushback els considera?)
      Pos p = posicions.front();
      
      int dist = m[p.i][p.j];
      for (int k = 0;k<8;k++){//afegim els valors corresponents a les 8 caselles que envolten en valor que estem tractant
        Pos aux = p + Dir(k);
        if (pos_ok(aux) and m[aux.i][aux.j] == -2){
          if(cell(aux).type==2 or cell(aux).type==3 or cell(aux).type==4) m[aux.i][aux.j] = -1; //com que es pels mags, tampoc ens interessa rock
          else m[aux.i][aux.j] = dist+1;
          posicions.push(aux);
        }
      }
      posicions.pop();
      
    }
  }

  void distancies_enemics(Matrix& m){
    queue<Pos> posicions3;

    for(int i=0;i<60;i++){ //omplim la matriu amb 0 on enemic i ens guardem les posicions en un vector
      for(int j=0;j<60;j++){
        Pos aux;
        aux.i=i;
        aux.j=j;
        int idaux = cell(aux).id;
        if (idaux!=-1){
          int owner = unit(idaux).player;

          if((owner != me() and (unit(idaux).type == 0 or unit(idaux).type == 1))){
           m[i][j] = 0;
           posicions3.push(aux);
          }
        }
      }
    }
    while (not posicions3.empty()){ //recorrem la cua de posicions
      Pos p = posicions3.front();
      int dist = m[p.i][p.j];

      for (int k = 0;k<8;k++){//afegim els valors corresponents a les 8 caselles que envolten en valor que estem tractant
        Pos aux3 = p + Dir(k);
        if (pos_ok(aux3) and m[aux3.i][aux3.j] == -2){
          if(cell(aux3).type==2 or cell(aux3).type==3){
           m[aux3.i][aux3.j] = -1; //nomes considerem impossible si hi ha granit o abyss
          }
          else {
            m[aux3.i][aux3.j] = dist+1;
            posicions3.push(aux3);
          }
        }
      }
      posicions3.pop();
      
    }
  }

  void distancies_tresors(Matrix& m){
    queue<Pos> posicions2;

    for(int i=0;i<60;i++){ //omplim la matriu amb 0 on tresor i ens guardem les posicions en un vector
      for(int j=0;j<60;j++){
        Pos aux;
        aux.i=i;
        aux.j=j;
        if(cell(aux).treasure){
         m[i][j] = 0;
         posicions2.push(aux);
        }
      }
    }

    while (not posicions2.empty()){ //recorrem la cua de posicions
      Pos p = posicions2.front();
      int dist = m[p.i][p.j];

      for (int k = 0;k<8;k++){//afegim els valors corresponents a les 8 caselles que envolten en valor que estem tractant
        Pos aux3 = p + Dir(k);
        if (pos_ok(aux3) and m[aux3.i][aux3.j] == -2){
          if(cell(aux3).type==2 or cell(aux3).type==3){
           m[aux3.i][aux3.j] = -1; //nomes considerem impossible si hi ha granit o abyss
          }
          else {
            m[aux3.i][aux3.j] = dist+1;
            posicions2.push(aux3);
          }
        }
      }
      posicions2.pop();
      
    }
  }




  /**
   * Play method, invoked once per each round.
   */
  virtual void play () {
    move_dwarves();
    move_wizards();
  }


};


/**
 * Do not modify the following line.
 */
RegisterPlayer(PLAYER_NAME);
