/**
 * Author: Ali Salloum
 * Problem: Accounting for a café
 * Date: 20-02-2021
 */
#include <iostream>
#include <exception>
using namespace std;
typedef long long ll;
const int MOD = 50021; /// the constant MOD that is used in the hashing algorithm. It must be prime and larger than the number of possible entries

/** @brief A function to raise x to the power n and return it modulo the constant MOD (500021)
 * @param x the base
 * @param n the power
 * @return (x^n)%MOD
 */
int power(ll x,int n) {
    ll re=1;
    for (int i=0;i<n;i++) re=(re*x)%MOD;
    return re;
}
/**
 * A class to store day, month, and year of a date and
 * do several operations on it
 */
class Date {
private:
    int day,month,year;
public:
    Date(){} /// default constructor
    /** @brief A parameter constructor to set the attributes of the date
     *
     * @param a day
     * @param b month
     * @param c year
     */
    Date(int a,int b,int c) {
        day=a;
        month=b;
        year=c;
    }
    /** @brief A function to convert a number to a string
     *
     * @param n the number to convert to string
     * @return ret a string that represents the number in decimal representation
     *
     */
    string to_string(int n) {
        string temp,ret;
        while (n) {
            char c = (n%10)+'0';
            temp+=c;
            n/=10;
        }
        /// we need to reverse the string since it's stored in temp from left to right
        for (int i=temp.size()-1;i>=0;i--) {
            ret+=temp[i];
        }
        return ret;
    }
    /** @brief Hash code of current date to store it in a hash table
     *
     * @return the hash code
     *
     */
    int hashCode() {
        int day_num = day+(month-1)*31+(year-1)*372;/// we deduce the number of the day to compress the length of the date
        string s = to_string(day_num);
        ll re = 0;
        int p = 13;
        /// here is the implementation of polynomial rolling hash function
        for (int i=0;i<s.size();i++) {
            re=(re+(s[i]-'0'+1)*power(p,i))%MOD;
        }
        re%=MOD;
        return re;
    }
    /// this function is used to fill cells after removing it from the hash table in order to keep
    /// the clusters connected when using linear probing in the hash table
    Date DEFUNCT() {
        return Date(-1,-1,-1);
    }
    /// the default starting value of the date
    Date zero() {
        return Date(0,0,0);
    }
    /// to plot the date in the following form : YYYY-MM-DD
    string show() {
        string re="";
        if (year<10) re+="0";/// we add a zero if the number consists of one digit
        re=to_string(year);
        re+="-";
        if (month<10) re+="0";
        re+=to_string(month);
        re+="-";
        if (day<10) re+="0";
        re+=to_string(day);
        return re;
    }
    /// overloading the operator in order to compare dates
    bool operator !=(const Date& d) {
        return d.day!=day || d.month!=month || d.year!=year;
    }
};

/** @brief A class to organize customers by their IDs and the date of the visit to count how many for each date
 */
class Customer {
private:
    string ID;
    Date date;
    int p=41;/// we choose a prime number that is roughly equal to the number of different characters
    /** @brief the same hash function as the one for dates but here there are more possible values to
     * which are (A -> Z , 0 -> 9 and '-')
     * @param s the string to get the hash code from
     * @return the hash code
     *
     */
    int hash_func(string s) {
        ll re=0;
        /// hash = (s[0]*P^{0} + s[1]*P^{1} + ....s[m]*P^{m}) mod M
        for (int i=0;i<s.size();i++) {
            if (s[i]=='-') re=(re+(1)*power(p,i))%MOD;
            else if (s[i]>='A' && s[i]<='Z') re=(re+(s[i]-'A'+2)*power(p,i))%MOD;
            else re=(re+(s[i]-'0'+28)*power(p,i))%MOD;
        }
        re%=MOD;
        return re;
    }
public:
    Customer(){}/// default constructor
    /** @brief A parameter constructor to set the starting values for a customer
     *
     * @param i string the ID of the customer
     * @param d Date the date of the visit of the customer
     *
     */
    Customer(string i,Date d) {
        date=d;
        ID=i;
    }
    /** @brief A function that converts the customer to a string and calculates its hash code
     *
     * \return int the hash code
     *
     */
    int hashCode() {
        string cur=date.to_string(date.hashCode());
        cur+=ID;
        return hash_func(cur);
    }
    /** @brief this function is used to fill cells after removing it from the hash table in order to keep
     * the clusters connected when using linear probing in the hash table
     *
     * @return Customer a customer with unique details
     *
     */
    Customer DEFUNCT() {
        Customer c = Customer("|",Date().DEFUNCT());
        return c;
    }
    /** @brief the default starting value of the customer
     *
     * @return Customer A customer with zero date and unique ID
     *
     */
    Customer zero() {
        Customer c = Customer("z",Date().zero());
        return c;
    }
    /** @brief A function to compare customers if they are not equal
     *
     * @return bool operator not equal or equals
     *
     */
    bool operator !=(const Customer& c) {
        Date r = c.date;
        return c.ID!=ID || r!=date;
    }
};

/// An abstract class to map generic keys with values
template <class T,class U>
class Map {
    virtual U get(T key) = 0;
    virtual int add(T key,U value) = 0;
    virtual void remove(T key) = 0;
    virtual int indexOf(T key) = 0;
    virtual bool exist(T key) = 0;
    virtual int size() = 0;
    virtual void setIndex(int index,U value) = 0;
    virtual pair<T,U> atIndex(int index) = 0;

};

/// A hash table class that inherits from Map class
template <class T,class U>
class HashTable : Map<T,U> {
private:
    pair<T,U> *table = new pair<T,U>[60000];
    int entries=0;
public:
    HashTable<T,U> (){
        T x;
        for (int i=0;i<55000;i++) table[i].first=x.zero();
    }
    /** @brief get the value of the key
     *
     * @param key T the key
     * @return U the value in the hash table
     *
     */
    U get(T key) {
        int index=key.hashCode();
        while (table[index].first!=key) {
            if (!(table[index].first!=key.zero())) break;
            index++;
            index%=MOD;
        }
        return table[index].second;
    }
    /** @brief Add to the hash table
     *
     * @param key T the key
     * @param value U the value
     * @return int the index
     *
     */
    int add(T key,U value) {
        int index = key.hashCode();
        while (table[index].first!=key.zero() && table[index].first!=key.DEFUNCT()) {
            index++;
            index%=MOD;
        }
        table[index]={key,value};
        entries++;
        return index;
    }
    /** @brief remove pair {key,value} from the hash table
     *
     * @param key T the key to be removed
     * @return void
     *
     */
    void remove(T key) {
        entries--;
        int index=key.hashCode();
        while (table[index].first!=key.zero() && table[index].first!=key.DEFUNCT()) {
            index++;
            index%=MOD;
        }
        table[index].first=key.DEFUNCT();
    }
    /** @brief get the index of the key
     *
     * \param key T the key to get the index of
     * \return int the index of the key
     *
     */
    int indexOf(T key) {
        int index=key.hashCode();
        while (table[index].first!=key) {
            if (!(table[index].first!=key.zero())) break;
            index++;
            index%=MOD;
        }
        return index;
    }
    /** @brief To check whether a key exists in the hash table
     *
     * \param key T the key to check the existance of
     * \return bool true if it exists and false otherwise
     *
     */
    bool exist(T key) {
        if (!(key!=key.zero()) || !(key!=key.DEFUNCT())) return false;
        int index=key.hashCode();
        while (table[index].first!=key) {
            if (!(table[index].first!=key.zero())) break;
            index++;
            index%=MOD;
        }
        if (table[index].first!=key.DEFUNCT() && table[index].first!=key.zero()) return true;
        else return false;
    }
    /** @brief the number of entries in the hash table
     *
     * @return int the size
     *
     */
    int size() {
        return entries;
    }
    void setIndex(int index,U value) {
        table[index].second=value;
    }
    pair<T,U> atIndex(int index) {
        return table[index];
    }
};

int main()
{
    HashTable<Date,double> money;
    HashTable<Date,int> unique;
    HashTable<Customer,bool> visited;
    int n,day,month,year;
    Date date;
    string time,ID,item;
    char h,d;
    double price;
    cin>>n;/// the number of entries in the input
    for (int i=0;i<n;i++) {
        try {
            scanf("%d-%d-%d",&year,&month,&day);
            if (year<0 || month<0 || day<0) throw ;
        } catch (...) {
            cout<<"The date cannot contain negative values\n";
            continue;
        }
        date=Date(day,month,year);
        cin>>time;
        cin>>h>>ID;
        try {
            cin>>d>>price;
            if (price<0) throw ;
        } catch (...) {
            cout<<"The price cannot be a negative value\n";
            continue;
        }
        getline(cin,item);
        if (money.exist(date)) {
            int j=money.indexOf(date);
            money.setIndex(j,money.get(date)+price);///increase the price
        }
        else {
            money.add(date,price);/// add a new entry
        }
        Customer c = Customer(ID,date);
        if (!visited.exist(c)) {/// if it is the first time to occur
            visited.add(c,true);
            if (unique.exist(date)) {
                int j=unique.indexOf(date);
                unique.setIndex(j,unique.get(date)+1);
            }
            else {
                unique.add(date,1);
            }
        }
    }
    /// show the results
    for (int i=0;i<55000;i++) {
        pair<Date,double> p = money.atIndex(i);
        date = p.first;
        if (money.exist(date)) {
            cout<<date.show()<<" $"<<p.second<<' '<<unique.get(date)<<endl;
        }
    }
}
