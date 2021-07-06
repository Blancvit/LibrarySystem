#include <iostream>
#include <string>
#include <fstream>

using namespace std;
string bookNum, title, author, available, borrowed, searchNum, userName;
int login = 1;

//FUNCTIONS
void addBook();
void editBook();
void deleteBook();
void borrowBook();
void returnBook();
void viewAllBooks();
void deleteAllBooks();
void clearScreen();
void pressKey();

//DESIGN
void librarySystemDesign();
void libraryMenuDesign();
void addBookDesign();
void editBookDesign();
void deleteBookDesign();
void borrowBookDesign();
void returnBookDesign();
void viewAllBookDesign();
void deleteAllBookDesign();

void loginScreen() {
    clearScreen();
    librarySystemDesign();
    cout << "\n(Case-sensitive)\n Enter Username: ";
    getline(cin, userName);
    if (userName.empty() == true) {
        cout << "\nPlease put a username.";
        pressKey();
        clearScreen();
        loginScreen();
    }
    login = 0;
}

//MENU
int main(){
    int choice;
    if (login == 1)
        loginScreen();
    clearScreen();
    librarySystemDesign();
    libraryMenuDesign();

    cout << "                                 ENTER CHOICE: ";
    cin >> choice;

    while (!cin){
        cin.clear();                    //reset cin
        cin.ignore (100, '\n');        //clear user input
        cout << "\n                               ENTER CHOICE (1-8): ";
        cin >> choice;
    }

    getchar();
    clearScreen();

    switch (choice){
        case 1:
            addBook();
            break;
        case 2:
            editBook();
            break;
        case 3:
            deleteBook();
            break;
        case 4:
            borrowBook();
            break;
        case 5:
            returnBook();
            break;
        case 6:
            viewAllBooks();
            break;
        case 7:
            deleteAllBooks();
            break;
        case 8:
            exit(0);
            break;
        default:
            main();
            break;
    }

    return 0;
}

bool isDigit(char ch) { 
    if (ch >= '0' && ch <= '9') 
        return true; 
    else
        return false; 
} 

void clearScreen(){     //Do not want to use system("CLS");
    int n;
    for (n = 0; n < 10; n++)
        printf( "\n\n\n\n\n\n\n\n\n\n" );
}

void pressKey(){
    do 
    {
        cout << '\n' << "\nPress ENTER to continue...";
    } while (cin.get() != '\n');
}


void addBook(){
	addBookDesign();
	cout << "(Pick any combination of number or letters you want)\nAssign a Book Reference Number: ";
	getline(cin, bookNum);
	cout << "Enter Title: ";
	getline(cin, title);
	cout << "Enter Author: ";
	getline(cin, author);
    cout << "Enter Quantity: ";
    getline(cin, available);
    for (int i = 0; i < available.length(); i++) {
        if (isDigit(available[i]) == true) {
            if (available.length() < 9)
                continue;
            else
                cout << "\nQuantity is too big!";
                pressKey();
                main();
        } else {
            cout << "\nOnly input numbers in quantity!";
            pressKey();
            main();
        }
    }

    if (bookNum.empty() == true || title.empty() == true || author.empty() == true || available.empty() == true) {
        cout << "\nPlease fill in all the fields.";
        pressKey();
        clearScreen();
        main();
    }

    string bookNumTxt = bookNum + ".txt";
    if (ifstream(bookNumTxt)) {
        cout << "\nThere is already a book with that number!";
        pressKey();
        main();
    } else {
        ofstream bookSingle(bookNumTxt, ios::app);
        bookSingle << "Book Reference Number: " << bookNum << "\nTitle: " << title << "\nAuthor: " << author << "\nAvailable: " << available << "\nBorrowed: 0\n";
        bookSingle.close();
        ofstream bookRec("Book Records.txt", ios::app);
        bookRec << "Book Reference Number: " << bookNum << "\nTitle: " << title << "\nAuthor: " << author << "\nAvailable: " << available << "\nBorrowed: 0\n";
        bookRec.close();
	    cout << "\nThe book has been added!";
	    pressKey();
        main();
    }
}

void deleteBook(){
    string bookEntry;
    int choice;
    deleteBookDesign();
	cout << "Enter Book Reference Number: ";
	getline(cin, searchNum);
    
    ifstream bookSearch(searchNum + ".txt");
    if (bookSearch.is_open()) {
        cout << "\nBook found!\n\n" << bookSearch.rdbuf() << endl;
        bookSearch.close();
        cout << "Do you want to delete this book?\n[1] Yes\n[2] No\nYour choice: ";
        cin >> choice;

        if (choice == 1) {
            //GET INFO AND DELETE SINGLE BOOK
            string line;
            string searchNumTxt = searchNum + ".txt";
            ifstream openFile(searchNumTxt);
            getline(openFile, bookNum); //gets first line, stores in bookNum
            getline(openFile, title);
            getline(openFile, author);
            getline(openFile, available);
            getline(openFile, borrowed);
            openFile.close();
            remove(searchNumTxt.c_str()); //convert string into const char[] for remove() to work

            //EDIT BOOK RECORD
            ifstream bookRecord("Book Records.txt");
            if( !bookRecord.is_open()) {
                cout << "File failed to open.";
                pressKey();
                main();
            }
            ofstream bookTemp("temp.txt");
            string del = bookNum;
            while (getline(bookRecord, line)) {
                if ( del == line )
                    for (int i = 0; i < 4; i++)
                        getline(bookRecord, line);
                else
                    bookTemp << line << endl;
            }

            bookRecord.close();
            bookTemp.close();
            remove("Book Records.txt");
            rename("temp.txt","Book Records.txt");

            getchar();
            cout << "\nBook has been deleted!";
            pressKey();
            main();
        } else {
            getchar();
            cout << "\nDelete cancelled.";
            pressKey();
            main();
        }
    }
    else
        cout << "\nBook Reference Number could not be found!";
        pressKey();
        main();
}

void revertEdit() {
    ofstream bookSingle(searchNum + ".txt", ios::app);
    bookSingle << bookNum << "\n" << title << "\n" << author << "\n" << available << "\n" << borrowed << "\n";
    bookSingle.close();
    ofstream bookRec("Book Records.txt", ios::app);
    bookRec << bookNum << "\n" << title << "\n" << author << "\n" << available << "\n" << borrowed << "\n";
    bookRec.close();
}

void editBook(){
    string bookEntry;
    int choice;
    editBookDesign();
	cout << "Enter Book Reference Number: ";
	getline(cin, searchNum);
    
    ifstream bookSearch(searchNum + ".txt");
    if (bookSearch.is_open()) {
        cout << "\nBook found!\n\n" << bookSearch.rdbuf() << endl;
        bookSearch.close();
        cout << "Do you want to edit this book?\n[1] Yes\n[2] No\nYour choice: ";
        cin >> choice;

        if (choice == 1) {
            //GET INFO AND DELETE SINGLE BOOK
            string line;
            string searchNumTxt = searchNum + ".txt";
            ifstream openFile(searchNumTxt);
            getline(openFile, bookNum); //gets first line, stores in bookNum
            getline(openFile, title);
            getline(openFile, author);
            getline(openFile, available);
            getline(openFile, borrowed);
            openFile.close();
            remove(searchNumTxt.c_str()); //convert string into const char[] for remove() to work

            //EDIT BOOK RECORD
            ifstream bookRecord("Book Records.txt");
            if( !bookRecord.is_open()) {
                cout << "File failed to open.";
                pressKey();
                main();
            }
            ofstream bookTemp("temp.txt");
            string del = bookNum;
            while (getline(bookRecord, line)) {
                if ( del == line )
                    for (int i = 0; i < 4; i++)
                        getline(bookRecord, line);
                else
                    bookTemp << line << endl;
            }

            bookRecord.close();
            bookTemp.close();
            remove("Book Records.txt");
            rename("temp.txt","Book Records.txt");

            //ASK WHICH PART TO BE EDITED
            string title2, author2, available2;
            cout << "\nWhat do you want to edit?\n[1] Title\n[2] Author\n[3] Quantity\nYour choice: ";
            cin >> choice;
            getchar();
            switch (choice){
                case 1:
                    cout << "Enter New Title: ";
                    getline(cin, title2);
                    if (title2.empty() == true) {
                        cout << "\nPlease do not leave it blank.";
                        revertEdit();
                        pressKey();
                        main();
                    }
                    break;
                case 2:
                    cout << "Enter New Author: ";
                    getline(cin, author2);
                    if (author2.empty() == true) {
                        cout << "\nPlease do not leave it blank.";
                        revertEdit();
                        pressKey();
                        main();
                    }
                    break;
                case 3:
                    cout << "Enter New Quantity: ";
                    getline(cin, available2);
                    for (int i = 0; i < available2.length(); i++) {
                        if (isDigit(available2[i]) == true) {
                            if (available2.length() < 9)
                                continue;
                            else
                                cout << "\nQuantity is too big!";
                                revertEdit();
                                pressKey();
                                main();
                        } else {
                            cout << "\nOnly input numbers in quantity!";
                            revertEdit();
                            pressKey();
                            main();
                        }
                    }
                    if (available2.empty() == true) {
                        cout << "\nPlease do not leave it blank.";
                        revertEdit();
                        pressKey();
                        main();
                    }
                    break;
                default:
                    cout << "\nInput 1-3 based on your choice!";
                    revertEdit();
                    pressKey();
                    main();
            }

            switch (choice){
                case 1:
                {
                    string searchNumTxt = searchNum + ".txt";
                    ofstream bookSingle(searchNumTxt, ios::app);
                    bookSingle << bookNum << "\nTitle: " << title2 << "\n" << author << "\n" << available << "\n" << borrowed << "\n";
                    bookSingle.close();
                    ofstream bookRec("Book Records.txt", ios::app);
                    bookRec << bookNum << "\nTitle: " << title2 << "\n" << author << "\n" << available << "\n" << borrowed << "\n";
                    bookRec.close();
                    break;
                }
                case 2:
                {
                    string searchNumTxt = searchNum + ".txt";
                    ofstream bookSingle(searchNumTxt, ios::app);
                    bookSingle << bookNum << "\n" << title << "\nAuthor: " << author2 << "\n" << available << "\n" << borrowed << "\n";
                    bookSingle.close();
                    ofstream bookRec("Book Records.txt", ios::app);
                    bookRec << bookNum << "\n" << title << "\nAuthor: " << author2 << "\n" << available << "\n" << borrowed << "\n";
                    bookRec.close();
                    break;
                }
                case 3:
                {
                    string searchNumTxt = searchNum + ".txt";
                    ofstream bookSingle(searchNumTxt, ios::app);
                    bookSingle << bookNum << "\n" << title << "\n" << author << "\nAvailable: " << available2 << "\n" << borrowed << "\n";
                    bookSingle.close();
                    ofstream bookRec("Book Records.txt", ios::app);
                    bookRec << bookNum << "\n" << title << "\n" << author << "\nAvailable: " << available2 << "\n" << borrowed << "\n";
                    bookRec.close();
                    break;
                }
            }

            cout << "\nBook has been edited!";
            pressKey();
            main();
        } else {
            getchar();
            cout << "\nEdit cancelled.";
            pressKey();
            main();
        }
    }
    else
        cout << "\nBook Reference Number could not be found!";
        pressKey();
        main();
}

void borrowBook(){
    string bookEntry, checkLine, getUserBorrow = "0";
    int choice, borrowTotalUser;
    borrowBookDesign();
	cout << "Enter Book Reference Number: ";
	getline(cin, searchNum);
    
    ifstream bookSearch(searchNum + ".txt");
    if (bookSearch.is_open()) {
        cout << "\nBook found!\n\n" << bookSearch.rdbuf() << endl;
        bookSearch.close();
        cout << "Do you want to borrow this book?\n[1] Yes\n[2] No\nYour choice: ";
        cin >> choice;

        if (choice == 1) {
            string searchNumTxt = searchNum + ".txt";
            ifstream openFile(searchNumTxt);
            getline(openFile, bookNum);
            getline(openFile, title);
            getline(openFile, author);
            getline(openFile, available);
            getline(openFile, borrowed);
            while (getline(openFile, checkLine)){
                if (checkLine == userName){
                    getline(openFile, checkLine);
                    getUserBorrow = checkLine;
                    getUserBorrow.erase (0, 15);
                }
            }
            openFile.close();
            available.erase (0, 11); //erase 11 characters from "Available: x" starting at position 0
            borrowed.erase (0, 10);
            int availableInt = stoi(available), borrowedInt = stoi(borrowed), borrow, borrowTotalUser = stoi(getUserBorrow); //convert string into integers
            cout << "How many would you like to borrow? ";
            cin >> borrow;
            if (borrow < 0){
            cout << "\nYou cannot borrow negative books!";
            getchar();
            pressKey();
            main();
            }
            availableInt -= borrow;
            if (availableInt < 0){
                cout << "\nYou are borrowing too much copies!";
                getchar();
                pressKey();
                main();
            }
            int borrowTotal = borrow + borrowedInt;
            borrowTotalUser += borrow;
            string available2 = "Available: " + to_string(availableInt);

            //EDIT SINGLE BOOK AND BORROW RECORD
            string line;
            bookNum.erase (0, 23);
            ifstream bookSingleRead(bookNum + ".txt");
            if( !bookSingleRead.is_open()){
                cout << "\nFile failed to open.";
                pressKey();
                main();
            }
            ofstream bookSingle("temp.txt", ios::app);
            bookNum = "Book Reference Number: " + bookNum;
            bookSingle << bookNum << "\n" << title << "\n" << author << "\n" << available2 << "\nBorrowed: " << borrowTotal << "\n";
            for (int i = 0; i < 5; i++)
                    getline(bookSingleRead, line);
            while(getline(bookSingleRead, line)) {
                if (line == userName)
                    getline(bookSingleRead, line);
                else
                    bookSingle << line << endl;
            }
            bookSingle << userName << "\nUser Borrowed: " << borrowTotalUser << "\n";
            bookSingleRead.close();
            bookSingle.close();
            remove(searchNumTxt.c_str());
            rename("temp.txt", searchNumTxt.c_str());
            
            //EDIT BOOK RECORD
            ifstream bookRecord("Book Records.txt");
            if( !bookRecord.is_open()) {
                cout << "\nFile failed to open.";
                pressKey();
                main();
            }
            ofstream bookTemp("temp.txt");
            string del = bookNum;
            while (getline(bookRecord, line)) {
                if ( del == line )
                    for (int i = 0; i < 4; i++)
                        getline(bookRecord, line);
                else
                    bookTemp << line << endl;
            }

            bookRecord.close();
            bookTemp.close();
            remove("Book Records.txt");
            rename("temp.txt","Book Records.txt");
            ofstream bookBorrowed("Book Records.txt", ios::app);
            bookBorrowed << bookNum << "\n" << title << "\n" << author << "\n" << available2 << "\nBorrowed: " << borrowTotal << "\n";
            bookBorrowed.close();

            getchar();
            cout << "\nBook has been borrowed.";
            pressKey();
            main();
        } else {
            getchar();
            cout << "\nBorrow cancelled.";
            pressKey();
            main();
        }
    } else
        cout << "\nBook Reference Number could not be found!.";
    pressKey();
    main();
}

void returnBook(){
    string bookEntry, checkLine, getUserBorrow;
    int choice, checkRecord = 1;
    returnBookDesign();
	cout << "Enter Book Reference Number: ";
	getline(cin, searchNum);
    
    ifstream bookSearch(searchNum + ".txt");
    if (bookSearch.is_open()) {
        cout << "\nBook found!\n\n" << bookSearch.rdbuf() << endl;
        bookSearch.close();
        cout << "Do you want to return this book?\n[1] Yes\n[2] No\nYour choice: ";
        cin >> choice;

        if (choice == 1) {
            string searchNumTxt = searchNum + ".txt";
            ifstream openFile(searchNumTxt);
            getline(openFile, bookNum);
            getline(openFile, title);
            getline(openFile, author);
            getline(openFile, available);
            getline(openFile, borrowed);
            while (getline(openFile, checkLine)){
                if (checkLine == userName){
                    getline(openFile, checkLine);
                    getUserBorrow = checkLine;
                    getUserBorrow.erase (0, 15);
                    checkRecord = 0;
                }
            }
            if (checkRecord == 1){
                cout << "\nUser " << userName << " has not borrowed yet!";
                getchar();
                pressKey();
                main();
            }
            openFile.close();
            available.erase (0, 11); 
            borrowed.erase (0, 10);
            int availableInt = stoi(available), borrowedInt = stoi(borrowed), returnBook, borrowTotalUser = stoi(getUserBorrow);;
            if (borrowedInt == 0){
                cout << "\nYou have not borrowed this book yet!";
                getchar();
                pressKey();
                main();
            }
            cout << "How many would you like to return? ";
            cin >> returnBook;
            if (returnBook < 0){
            cout << "\nYou cannot return negative books!";
            getchar();
            pressKey();
            main();
            }
            availableInt += returnBook;
            borrowedInt -= returnBook;
            if (borrowedInt < 0){
                cout << "\nYou do not have that many copies!";
                getchar();
                pressKey();
                main();
            }
            borrowTotalUser -= returnBook;
            if (borrowTotalUser < 0){
                cout << "\nUser " << userName << " does not have that many copies!";
                getchar();
                pressKey();
                main();
            }
            string available2 = "Available: " + to_string(availableInt);

            //EDIT BOOK AND BORROW RECORD
            string line;
            bookNum.erase (0, 23);
            ifstream bookSingleRead(bookNum + ".txt");
            if( !bookSingleRead.is_open()){
                cout << "\nFile failed to open.";
                pressKey();
                main();
            }  

            ofstream bookSingle("temp.txt", ios::app);
            bookNum = "Book Reference Number: " + bookNum;
            bookSingle << bookNum << "\n" << title << "\n" << author << "\n" << available2 << "\nBorrowed: " << borrowedInt << "\n";
            for (int i = 0; i < 5; i++)
                    getline(bookSingleRead, line);
            while(getline(bookSingleRead, line)) {
                if (line == userName)
                    getline(bookSingleRead, line);
                else
                    bookSingle << line << endl;
            }
            if (borrowTotalUser != 0){
                bookSingle << userName << "\nUser Borrowed: " << borrowTotalUser << "\n";
            }
            bookSingleRead.close();
            bookSingle.close();
            remove(searchNumTxt.c_str());
            rename("temp.txt", searchNumTxt.c_str());

            //EDIT BOOK RECORD
            ifstream bookRecord("Book Records.txt");
            if( !bookRecord.is_open()) {
                cout << "\nFile failed to open.";
                pressKey();
                main();
            }
            ofstream bookTemp("temp.txt");
            string del = bookNum;
            while (getline(bookRecord, line)) {
                if ( del == line )
                    for (int i = 0; i < 4; i++)
                        getline(bookRecord, line);
                else
                    bookTemp << line << endl;
            }

            bookRecord.close();
            bookTemp.close();
            remove("Book Records.txt");
            rename("temp.txt","Book Records.txt");
            ofstream bookBorrowed("Book Records.txt", ios::app);
            bookBorrowed << bookNum << "\n" << title << "\n" << author << "\n" << available2 << "\nBorrowed: " << borrowedInt<< "\n";
            bookBorrowed.close();

            getchar();
            cout << "\nBook has been returned.";
            pressKey();
            main();
        } else {
            getchar();
            cout << "\nReturn cancelled.";
            pressKey();
            main();
        }
    } else
        cout << "\nBook Reference Number could not be found!.";
    pressKey();
    main();
}

void viewAllBooks(){
    viewAllBookDesign();
    ifstream allData("Book Records.txt");
    if (allData.is_open()) //is_open is from fstream
        cout << allData.rdbuf() << "\nAll books have been displayed!"; 
    else
        cout << "\nThere are no book records yet!";
    
    allData.close();
    pressKey();
    main();
}

void deleteAllBooks(){
    int choice;
    ifstream allData ("Book Records.txt");
    deleteAllBookDesign();
    cout << "Do you wish to delete all books?\n[1]Yes\n[2]No\nYour choice: ";
    cin >> choice;
    if (choice == 1) {
        if (allData.is_open()) {
            while (getline(allData, bookNum)) {
                bookNum.erase (0, 23);
                string bookNumTxt = bookNum + ".txt";
                remove(bookNumTxt.c_str());
                for (int i = 0; i < 4; i++)
                    getline(allData, bookNum);
            }
            allData.close();
            remove("Book Records.txt");
            cout << "\nAll books have been deleted!";
            getchar();
            pressKey();
            main();
        } else
            allData.close();
            getchar();
            cout << "\nThere are no books to delete yet!";
    } else {
        getchar();
        cout << "\nDelete all books cancelled.";    
    }
    pressKey();
    main();
}

//DESIGN SECTION (UNRELATED TO CODE)
void librarySystemDesign() {
    cout << R"(
      __    _ __                             _____            __               
     / /   (_/ /_  _________ ________  __   / ___/__  _______/ /____  ____ ___ 
    / /   / / __ \/ ___/ __ `/ ___/ / / /   \__ \/ / / / ___/ __/ _ \/ __ `__ \
   / /___/ / /_/ / /  / /_/ / /  / /_/ /   ___/ / /_/ (__  / /_/  __/ / / / / /
  /_____/_/_.___/_/   \__,_/_/   \__, /   /____/\__, /____/\__/\___/_/ /_/ /_/ 
                                /____/         /____/                                
    )" << '\n';
}

void libraryMenuDesign() {
    cout << ".-=~=-.                                                                 .-=~=-.\n";
    cout << "(__  _)-._.-=-._.-=-._.-=-._.-=-._.-=-._.-=-._.-=-._.-=-._.-=-._.-=-._.-(__  _)\n";
    cout << "( _ __)                           [1] ADD BOOK                          ( _ __)\n";	
    cout << "(__  _)                           [2] EDIT BOOK                         (__  _)\n";	
    cout << "(_ ___)                          [3] DELETE BOOK                        (_ ___)\n";	
    cout << "(__  _)                          [4] BORROW BOOK                        (__  _)\n";	
    cout << "( _ __)                          [5] RETURN BOOK                        ( _ __)\n";
    cout << "(__  _)                        [6] VIEW ALL BOOKS                       (__  _)\n";	
    cout << "(_ ___)                       [7] DELETE ALL BOOKS                      (_ ___)\n";
    cout << "(__  _)                              [8] EXIT                           (__  _)\n";
    cout << "( _ __)-._.-=-._.-=-._.-=-._.-=-._.-=-._.-=-._.-=-._.-=-._.-=-._.-=-._.-(_ ___)\n";
    cout << "`-._.-'                                                                 `-._.-'\n";
}

void addBookDesign() {
    cout << R"(
_.--.__.-'""`-.__.--.__.-'""`-.__.--.__.-'""`-.____.--.__.-'""`-.__.--.__.-'""`-.__.--._
"`--'""`-.__.-'""`--'""`-.__.-'""`--'""`-.__.-'""`"`--'""`-.__.-'""`--'""`-.__.-'""`--'"
    ___       __    __   ____              __  
   /   | ____/ ____/ /  / __ )____  ____  / /__
  / /| |/ __  / __  /  / __  / __ \/ __ \/ //_/
 / ___ / /_/ / /_/ /  / /_/ / /_/ / /_/ / ,<   
/_/  |_\__,_/\__,_/  /_____/\____/\____/_/|_| 

_.--.__.-'""`-.__.--.__.-'""`-.__.--.__.-'""`-.____.--.__.-'""`-.__.--.__.-'""`-.__.--._
"`--'""`-.__.-'""`--'""`-.__.-'""`--'""`-.__.-'""`"`--'""`-.__.-'""`--'""`-.__.-'""`--'"
    )" << '\n';
}

void editBookDesign() {
    cout << R"(
_.--.__.-'""`-.__.--.__.-'""`-.__.--.__.-'""`-.____.--.__.-'""`-.__.--.__.-'""`-.__.--._
"`--'""`-.__.-'""`--'""`-.__.-'""`--'""`-.__.-'""`"`--'""`-.__.-'""`--'""`-.__.-'""`--'"
    ______    ___ __     ____              __  
   / ________/ (_/ /_   / __ )____  ____  / /__
  / __/ / __  / / __/  / __  / __ \/ __ \/ //_/
 / /___/ /_/ / / /_   / /_/ / /_/ / /_/ / ,<   
/_____/\__,_/_/\__/  /_____/\____/\____/_/|_|

_.--.__.-'""`-.__.--.__.-'""`-.__.--.__.-'""`-.____.--.__.-'""`-.__.--.__.-'""`-.__.--._
"`--'""`-.__.-'""`--'""`-.__.-'""`--'""`-.__.-'""`"`--'""`-.__.-'""`--'""`-.__.-'""`--'"                
    )" << '\n';
}

void deleteBookDesign() {
    cout << R"(
_.--.__.-'""`-.__.--.__.-'""`-.__.--.__.-'""`-.____.--.__.-'""`-.__.--.__.-'""`-.__.--._
"`--'""`-.__.-'""`--'""`-.__.-'""`--'""`-.__.-'""`"`--'""`-.__.-'""`--'""`-.__.-'""`--'"
    ____       __     __          ____              __  
   / __ \___  / ___  / /____     / __ )____  ____  / /__
  / / / / _ \/ / _ \/ __/ _ \   / __  / __ \/ __ \/ //_/
 / /_/ /  __/ /  __/ /_/  __/  / /_/ / /_/ / /_/ / ,<   
/_____/\___/_/\___/\__/\___/  /_____/\____/\____/_/|_|  
_.--.__.-'""`-.__.--.__.-'""`-.__.--.__.-'""`-.____.--.__.-'""`-.__.--.__.-'""`-.__.--._
"`--'""`-.__.-'""`--'""`-.__.-'""`--'""`-.__.-'""`"`--'""`-.__.-'""`--'""`-.__.-'""`--'"                           
    )" << '\n';
}

void borrowBookDesign() {
    cout << R"(
_.--.__.-'""`-.__.--.__.-'""`-.__.--.__.-'""`-.____.--.__.-'""`-.__.--.__.-'""`-.__.--._
"`--'""`-.__.-'""`--'""`-.__.-'""`--'""`-.__.-'""`"`--'""`-.__.-'""`--'""`-.__.-'""`--'"
    ____                                   ____              __  
   / __ )____  ______________ _      __   / __ )____  ____  / /__
  / __  / __ \/ ___/ ___/ __ | | /| / /  / __  / __ \/ __ \/ //_/
 / /_/ / /_/ / /  / /  / /_/ | |/ |/ /  / /_/ / /_/ / /_/ / ,<   
/_____/\____/_/  /_/   \____/|__/|__/  /_____/\____/\____/_/|_|

_.--.__.-'""`-.__.--.__.-'""`-.__.--.__.-'""`-.____.--.__.-'""`-.__.--.__.-'""`-.__.--._
"`--'""`-.__.-'""`--'""`-.__.-'""`--'""`-.__.-'""`"`--'""`-.__.-'""`--'""`-.__.-'""`--'"
    )" << '\n';
}

void returnBookDesign() {
    cout << R"(
_.--.__.-'""`-.__.--.__.-'""`-.__.--.__.-'""`-.____.--.__.-'""`-.__.--.__.-'""`-.__.--._
"`--'""`-.__.-'""`--'""`-.__.-'""`--'""`-.__.-'""`"`--'""`-.__.-'""`--'""`-.__.-'""`--'"
    ____       __                      ____              __  
   / __ \___  / /___  ___________     / __ )____  ____  / /__
  / /_/ / _ \/ __/ / / / ___/ __ \   / __  / __ \/ __ \/ //_/
 / _, _/  __/ /_/ /_/ / /  / / / /  / /_/ / /_/ / /_/ / ,<   
/_/ |_|\___/\__/\__,_/_/  /_/ /_/  /_____/\____/\____/_/|_|

_.--.__.-'""`-.__.--.__.-'""`-.__.--.__.-'""`-.____.--.__.-'""`-.__.--.__.-'""`-.__.--._
"`--'""`-.__.-'""`--'""`-.__.-'""`--'""`-.__.-'""`"`--'""`-.__.-'""`--'""`-.__.-'""`--'"                                                      
    )" << '\n';
}

void viewAllBookDesign() {
    cout << R"(
_.--.__.-'""`-.__.--.__.-'""`-.__.--.__.-'""`-.____.--.__.-'""`-.__.--.__.-'""`-.__.--._
"`--'""`-.__.-'""`--'""`-.__.-'""`--'""`-.__.-'""`"`--'""`-.__.-'""`--'""`-.__.-'""`--'"
 _    ___                 ___    ____   ____              __       
| |  / (____ _      __   /   |  / / /  / __ )____  ____  / /_______
| | / / / _ | | /| / /  / /| | / / /  / __  / __ \/ __ \/ //_/ ___/
| |/ / /  __| |/ |/ /  / ___ |/ / /  / /_/ / /_/ / /_/ / ,< (__  ) 
|___/_/\___/|__/|__/  /_/  |_/_/_/  /_____/\____/\____/_/|_/____/  

_.--.__.-'""`-.__.--.__.-'""`-.__.--.__.-'""`-.____.--.__.-'""`-.__.--.__.-'""`-.__.--._
"`--'""`-.__.-'""`--'""`-.__.-'""`--'""`-.__.-'""`"`--'""`-.__.-'""`--'""`-.__.-'""`--'"                                                           
    )" << '\n';
}

void deleteAllBookDesign() {
    cout << R"(
_.--.__.-'""`-.__.--.__.-'""`-.__.--.__.-'""`-.____.--.__.-'""`-.__.--.__.-'""`-.__.--._
"`--'""`-.__.-'""`--'""`-.__.-'""`--'""`-.__.-'""`"`--'""`-.__.-'""`--'""`-.__.-'""`--'"
    ____       __     __          ___    ____   ____              __       
   / __ \___  / ___  / /____     /   |  / / /  / __ )____  ____  / /_______
  / / / / _ \/ / _ \/ __/ _ \   / /| | / / /  / __  / __ \/ __ \/ //_/ ___/
 / /_/ /  __/ /  __/ /_/  __/  / ___ |/ / /  / /_/ / /_/ / /_/ / ,< (__  ) 
/_____/\___/_/\___/\__/\___/  /_/  |_/_/_/  /_____/\____/\____/_/|_/____/  

_.--.__.-'""`-.__.--.__.-'""`-.__.--.__.-'""`-.____.--.__.-'""`-.__.--.__.-'""`-.__.--._
"`--'""`-.__.-'""`--'""`-.__.-'""`--'""`-.__.-'""`"`--'""`-.__.-'""`--'""`-.__.-'""`--'"                                                                              
    )" << '\n';
}
