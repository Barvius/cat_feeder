#ifndef task_h
#define task_h

class Task {
private:
  unsigned int h;
  unsigned int m;
  unsigned int weight;

public:
  Task(unsigned int h, unsigned int m, unsigned int weight){
    this->h = h;
    this->m = m;
    this->weight = weight;
  }

  unsigned int getHours(){
    return this->h;
  }

  unsigned int getMinutes(){
    return this->m;
  }

  unsigned int getWeight(){
    return this->weight;
  }

  void setWeight(unsigned int weight){
    this->weight = weight;
  }

  boolean isValid(){
    if(this->h < 24 && this->m < 60){
      return true;
    }
    return false;
  }
};

#endif
