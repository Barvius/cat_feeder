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

};

#endif
