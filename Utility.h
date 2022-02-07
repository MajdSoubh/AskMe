#ifndef UTILITY_H
#define UTILITY_H
#include <string>
#include <memory>
#include <tuple>
#include <ctime>
//this is generic compare class inheritance and used inside classes project 
//we could get rid of it if we use  < operator overloading with sort function  
class Comparable
{
public:
    virtual bool compare(std::shared_ptr<Comparable>) const = 0;
    virtual ~Comparable()
    {
    }
};

class Date
{

private:
    int min;
    int sec;
    int hour;
    int day;
    int mon;
    int year;

public:
    static Date Now();                  //return the current date right now
    Date(int, int, int, int, int, int); //parameterized constructor take sec,min,hour,day,mon,year
    int getSec() const;                 //return the current sec in this object
    int getMin() const;                 //return the current min in this object
    int getHour() const;                //return the current hour in this object
    int getDay() const;                 //return the current day in this object
    int getMon() const;                 //return the current month in this object
    int getYear() const;                //return the current year in this object
    void setSec(int);                   //take int , set second for current object
    void setMin(int);                   //take int , set minute for current object
    void setHour(int);                  //take int , set hour for current object
    void setDay(int);                   //take int , set day for current object
    void setMon(int);                   //take int , set month for current object
    void setYear(int);                  //take int , set year for current object
    bool operator<(const Date&) const; //return 1 if the current object date less than the passed object
};

#endif