/*
Title: Homework 4.2 A library record class with derived classes for books and CDs
Author: Anik Barua
Version: 1.0
Date: 05-04-2021

Description: This C++ program uses polymorphism. First we create a parent class "ItemRecord" and derive
two subclasses called "BookRecord" and "CDRecord". Then from main we create two objects from each subclasses
and store them in the ItemRecord pointer array. Then we test different functions like to_string() to test 
which class function it is calling. It is a example of a run-time polymorphism. 
*/

#include <iostream>
#include <string>
#include <vector>
#include <sstream>  
using namespace std;

/*
The ItemRecord class maintains books and other media records for a library. Its attributes 
are title(string), price(double), rating(character).
*/
class ItemRecord{
private:
    string title;
    double price;
    char rating;
public:
    //Constructor with parameter (if no parameter gets passed the default values gets assigned).
    ItemRecord(string x = "placeholder", double y = 0.99, char z = 'D'){
        //Checking for invariants
        if(y > 0){
            if((z >= 'A' && z <= 'D')){
                title = x;
                price = y;
                rating = z;
            } else {
                throw invalid_argument("Invalid Rating Letter!");
            }
        } else {
            throw invalid_argument("Negative Price!");
        }
    }

    //Getters
    string getTitle() { 
        return title;
    }

    double getPrice(){
        return price;
    }

    char getRating(){
        return rating;
    }

    //Overloaded “<<” operator which outputs the title only, implemented as a friend function
    friend ostream &operator<< (ostream &output, ItemRecord &item){
        output << item.title << endl;
        return output;
    }

    //Overloaded “==” operator which only compares the item's rating
    bool operator== (ItemRecord &item){
        if(rating == item.getRating()){
            return true;
        } else {
            return false;
        }
    }

    //setPrice(double) external function
    void setPrice(double newPrice);

    //a pure virtual function, to_string()
    virtual string to_string() = 0;

    //a virtual destructor
    virtual ~ItemRecord(){}
};

//setPrice(double) throws a range_error exception with the message “negative price”, if value < 0.
inline void ItemRecord::setPrice(double newPrice){
    if(newPrice < 0){
        throw range_error("Negative Price!");
    } else {
        price = newPrice;
    }
}

//BookRecord is a derived class from ItemRecord.
class BookRecord: public ItemRecord{
    private:
        int* chPages; //Implemented as a dynamic array of integers in the constructor
        int sizeChPages; //Size for the pointed dynamic array
    public:
        //Constructor with default values including for vector. Calling the parent constructor using subclass initialization list.
        BookRecord(string x = "placeholder", double y = 0.99, char z = 'D', vector<int> vec = vector<int>()) : ItemRecord(x, y, z){
            chPages = new int [vec.size()]; //Pointing to a new dynamic array of integers
            sizeChPages = vec.size(); //Initializing the size for chPages
            auto itr = vec.begin(); //Iterator to go through the vector values
            int i = 0;
            while(itr != vec.end()){
                chPages[i] = *itr; //Filling up the dynamic array with vector values
                i++;
                itr++;
            }
        }

        //display's the total # of pages for BookRecord including title, price and rating
        string to_string(){
            string price, totalPage;
            stringstream ss1, ss2; //To omit the zeros after decimal using string stream
            ss1 << ItemRecord::getPrice();
            price = ss1.str();
            int pages = 0; //counter for total number of pages
            int i = 0;
            while(i < sizeChPages){
                pages = pages + chPages[i];
                i++;
            }
            ss2 << pages;
            totalPage = ss2.str();
            return "Book[ " + ItemRecord::getTitle() + ", " + price + ", " + ItemRecord::getRating() + ", " + totalPage + " ]";
        }

        //Destructor to release the dynamic array 
        ~BookRecord(){
            delete[] chPages;
        }
};

//CdRecord is a derived class from ItemRecord.
class CdRecord: public ItemRecord{
    private:
        double* trackTimes; //Implemented as a dynamic array of doubles in the constructor
        int sizeTrackTimes; //Size for the pointed dynamic array
    public:
        //Constructor with default values including for vector. Calling the parent constructor using subclass initialization list.
        CdRecord(string x = "placeholder", double y = 0.99, char z = 'D', vector<double> vec = vector<double>()) : ItemRecord(x, y, z){
            trackTimes = new double [vec.size()]; //Pointing to a new dynamic array of double
            sizeTrackTimes = vec.size(); //Initializing the sizeTrackTimes
            auto itr = vec.begin(); //Iterator to go through the vector values
            int i = 0;
            while(itr != vec.end()){
                trackTimes[i] = *itr; //Filling up the dynamic array with vector values
                i++;
                itr++;
            }
        }

        //display's the total play time for CdRecord including title, price and rating
        string to_string(){
            string price, totalTime;
            stringstream ss1, ss2; //To omit the zeros after decimal using string stream
            ss1 << ItemRecord::getPrice();
            price = ss1.str();
            double time = 0; //counter for total minutes
            int i = 0;
            while(i < sizeTrackTimes){
                time = time + trackTimes[i];
                i++;
            }
            ss2 << time;
            totalTime = ss2.str(); //Converting int to string
            return "CD[ " + ItemRecord::getTitle() + ", " + price + ", " + ItemRecord::getRating() + ", " + totalTime + " ]";
        }

        //Destructor to release the dynamic array 
        ~CdRecord(){
            delete[] trackTimes;
        }
};

int main(){
    try{
        BookRecord book1 ("SampleBook", 10.99, 'A', {2, 20, 30, 40, 30, 20});
        CdRecord cd1 ("SampleCD", 14.99, 'B', {4.5, 15.0, 23.5, 4.3, 5.2, 20.3});

        //Displaying individual titles by using overloaded <<
        cout << "My book's title is: " << book1;
        cout << "My CD's title is: " << cd1;

        //Reducing each item's price by $1, using setPrice()
        book1.setPrice(book1.getPrice()-1);
        cd1.setPrice(cd1.getPrice()-1);

        //Showing whether the two items have the same rating by using overloaded ==
        cout << "Do they have the same rating? ";
        if(book1 == cd1){
            cout << "Yes\n" << endl;
        } else {
            cout << "No\n" << endl;
        }

        int size = 2;
        ItemRecord* items[size]; //Array of ItemRecord* to store the two items
        items[0] = &book1; 
        items[1] = &cd1;
        cout << "Their details in [title, price, rating, total pages/minutes]:\n";
        for(int i = 0; i < size; i++) {  
            cout << items[i]->to_string() << endl;
        }

    } catch (invalid_argument arg1){
        //invalid argument gets thrown when the rating letter is not between 'A' and 'D'.
        cout << arg1.what() << endl;
    } catch (range_error arg2){
        //range error gets thrown when negative price gets passed.
        cout << arg2.what() << endl;
    }
    return 0;
}