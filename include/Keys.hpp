#ifndef TOUCHES_HPP
#define TOUCHES_HPP

#include "declaration.hpp"

class Keys {
  private :
    bool z_key;
    bool s_key;
    bool q_key;
    bool d_key;
    bool space_key;
  public :
    Keys(void) :
      z_key(false),
      s_key(false),
      q_key(false),
      d_key(false),
      space_key(false) {}
    bool getZ_key(void){
      return z_key;
    }
    bool getQ_key(void){
      return q_key;
    }
    bool getS_key(void){
      return s_key;
    }
    bool getD_key(void){
      return d_key;
    }
    bool getSPACE_key(void){
      return space_key;
    }
    void setZ_key(bool b){
      z_key = b;
    }
    void setQ_key(bool b){
      q_key = b;
    }
    void setS_key(bool b){
      s_key = b;
    }
    void setD_key(bool b){
      d_key = b;
    }
    void setSPACE_key(bool b){
      space_key = b;
    }
    void setAllFalse(void){
      z_key = false;
      d_key = false;
      q_key = false;
      s_key = false;
      space_key = false;
    }
};
#endif
