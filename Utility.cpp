#include "Utility.h"
using namespace std;
Date Date::Now()
{
    time_t t = time(0);
    tm *now = localtime(&t);
    Date d = Date(now->tm_sec, now->tm_min, now->tm_hour, now->tm_mday, now->tm_mon + 1, now->tm_year + 1900);
    return d;
}
Date::Date(int _sec, int _min, int _hour, int _day, int _mon, int _year) : sec(_sec), min(_min), hour(_hour), day(_day), mon(_mon), year(_year)
{
}
int Date::getSec() const
{
    return sec;
}
int Date::getMin() const
{
    return min;
}
int Date::getHour() const
{
    return hour;
}
int Date::getDay() const
{
    return day;
}
int Date::getMon() const
{
    return mon;
}
int Date::getYear() const
{
    return year;
}
void Date::setSec(int _sec)
{
    sec = _sec;
}
void Date::setMin(int _min)
{
    min = _min;
}
void Date::setHour(int _hour)
{
    hour = _hour;
}
void Date::setDay(int _day)
{
    day = _day;
}
void Date::setMon(int _mon)
{
    mon = _mon;
}
void Date::setYear(int _year)
{
    year = _year;
}

bool Date::operator<(const Date &d) const
{
    int year_1 = getYear();
    int mon_1 = getMon();
    int day_1 = getDay();
    int hour_1 = getHour();
    int min_1 = getMin();
    int sec_1 = getSec();

    int year_2 = d.getYear();
    int mon_2 = d.getMon();
    int day_2 = d.getDay();
    int hour_2 = d.getHour();
    int min_2 = d.getMin();
    int sec_2 = d.getSec();

    return tie(year_1, mon_1, day_1, hour_1, min_1, sec_1) < tie(year_2, mon_2, day_2, hour_2, min_2, sec_2);
}