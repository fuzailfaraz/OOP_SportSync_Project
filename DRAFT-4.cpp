#include<iostream>
#include<iomanip>
#include<vector>
#include<limits>
#include<fstream>
#include<sstream>
#include <ctime>
using namespace std;

//ADMIN LOG
void logAdminAction(const string& action) {
    ofstream logFile("admin_log.txt", ios::app);
    time_t now = time(0);
    string dt = ctime(&now);
    dt.pop_back(); // remove newline character from ctime
    logFile << "[" << dt << "] " << action << endl;
    logFile.close();
} 

class User{         // Base abstract class
    protected:
    int ID;
    public:
    virtual void login() = 0;       //pure virtual funtion being overriden in derived classes
    virtual ~User() {}      //virtual destructor for safe polymorphism
};


struct MatchSlot {
    string day, date, time, event, team1, team2, venue;

    void display(int index) {
        cout << setw(2) << index + 1 << " | "
             << setw(9) << day << " | "
             << setw(10) << date << " | "
             << setw(11) << time << " | "
             << setw(12) << event << " | "
             << setw(5) << team1 << " vs " << setw(5) << team2 << " | "
             << setw(18) << venue << endl;
    }

    string toLine() {
        return day + "," + date + "," + time + "," + event + "," + team1 + "," + team2 + "," + venue;
    }

    static MatchSlot fromLine(string line) {
        MatchSlot m;
        stringstream ss(line);
        getline(ss, m.day, ',');
        getline(ss, m.date, ',');
        getline(ss, m.time, ',');
        getline(ss, m.event, ',');
        getline(ss, m.team1, ',');
        getline(ss, m.team2, ',');
        getline(ss, m.venue, ',');
        return m;
    }
};

string getVenueForSport(string sport) {
    if (sport == "Football" || sport == "Tug of War")
        return "Football Ground";
    else if (sport == "Futsal")
        return "Futsal Ground";
    else if (sport == "Badminton")
        return "Badminton Court";
    else if (sport == "Basketball")
        return "Basketball Court";
    else if (sport == "Tennis")
        return "Tennis Court";
    else
        return "General Ground";
}

class Player;

class ScheduleManager {
private:
    vector<MatchSlot> schedule;
    string filename = "schedule.txt";

public:
    ScheduleManager() {
        loadFromFile();
        if (schedule.empty()) preloadDefaultSchedule();
    }

    void viewSchedule() {
        if (schedule.empty()) {
            cout << "No matches scheduled.\n";
            return;
        }

        cout << "\nWeekly Match Schedule:\n";
        cout << "No | Day       | Date       | Time        | Event        | Match         | Venue\n";
        cout << "-------------------------------------------------------------------------------------------\n";

        for (int i = 0; i < schedule.size(); ++i) {
            schedule[i].display(i);
        }
    }

    //to view player team schedule
    void viewMySchedule(const Player& p);
    
     void addMatch() {
        MatchSlot m;
        cin.ignore();
        cout << "Enter Day: "; getline(cin, m.day);
        cout << "Enter Date (YYYY-MM-DD): "; getline(cin, m.date);
        cout << "Enter Time (HH:MM-HH:MM): "; getline(cin, m.time);
        cout << "Enter Event: "; getline(cin, m.event);
        cout << "Enter Team 1: "; getline(cin, m.team1);
        cout << "Enter Team 2: "; getline(cin, m.team2);
        m.venue = getVenueForSport(m.event);

        //overlapping detection---manahil
        for (int i = 0; i < schedule.size(); ++i) {
        const MatchSlot& existing = schedule[i];

    // Check same date and time
        if(existing.date==m.date && existing.time==m.time) {
            // checking for venue overlapping
            if(existing.venue ==m.venue) {
                cout<<"ERROR: Slot already booked at "<< m.venue <<" on "<< m.date <<" at "<< m.time << endl;
                return;
            }
            //checking for team overlapping
            if(existing.team1==m.team1 || existing.team2==m.team1 ||
                existing.team1==m.team2 || existing.team2==m.team2) {
                cout<<"\nERROR: One of or both of the teams are already scheduled to play at that time."<<endl;
                return;
            }
        }
}

        schedule.push_back(m);
        saveToFile();
        cout << "\nMatch added successfully."<<endl;
//admin log
        logAdminAction("Admin added a new match: " + m.team1 + " vs " + m.team2 + " on " + m.date + " at " + m.time);

    }

   void editMatch() {
        viewSchedule();
        if (schedule.empty()) return;

        int index;
        cout << "\nEnter match number to edit: ";
        cin >> index;
        if (index < 1 || index > schedule.size()) {
            cout << "\nInvalid match number."<<endl;
            return;
        }
        MatchSlot& m = schedule[index - 1];     //what does this mean, nvm
        
        int c; //choice for editing
        cout << "Editing Match #" << index << endl;
        cout<<"\nDo you want to edit: \n1.Day \n2.Date \n3.Time \n4.Event \n5.Team 1 \n6.Team 2"<<endl;
        cin>>c;
        cin.ignore(numeric_limits<streamsize>::max(), '\n');

        switch(c){
        case 1:
        cout << "Enter new Day: "; getline(cin, m.day);
        break;
        case 2:
        cout << "Enter new Date: "; getline(cin, m.date);
        break;
        case 3:
        cout << "Enter new Time: "; getline(cin, m.time);
        break;
        case 4:
        cout << "Enter new Event: "; getline(cin, m.event);
        break;
        case 5:
        cout << "Enter Team 1: "; getline(cin, m.team1);
        break;
        case 6:
        cout << "Enter Team 2: "; getline(cin, m.team2);
        break;
        default:
        cout<<"Invalid choice.\n";
        return;
        } 
        m.venue = getVenueForSport(m.event);

        //conflict checking excluding the match currently being edited
        for (int i = 0; i < schedule.size(); ++i) {
            if (i == index - 1) continue;   //skip the match being edited
           
            const MatchSlot& existing = schedule[i];

            if(existing.date == m.date && existing.time == m.time){
                //check venue conflict 
                if(existing.venue== m.venue){
                    cout<<"\nERROR: Slot is already booked at "<<m.venue<<" on "<<m.date<<" at "<<m.time<<"."<<endl;
                    return;
                }
                //checking for team conflict
                if(existing.team1 == m.team1 || existing.team2==m.team1 ||
                existing.team1 == m.team2 || existing.team2 == m.team2){
                    cout<<"\nERROR: One of or both of the teams are already booked at that time."<<endl;
                    return;
                }
            }
        }  

        saveToFile();
        cout << "\nMatch updated."<<endl;

        logAdminAction("Admin edited match #" + to_string(index));

    }

  void preloadDefaultSchedule() {
    schedule = {
        // Day 1 – 2023-04-13
        {"Thurs", "2023-04-13", "09:00-10:00", "Futsal", "CS", "EE", getVenueForSport("Futsal")},
        {"Thurs", "2023-04-13", "09:00-10:00", "Football", "MS", "ME", getVenueForSport("Football")},
        {"Thurs", "2023-04-13", "10:30-11:30", "Badminton", "SS", "AE", getVenueForSport("Badminton")},
        {"Thurs", "2023-04-13", "12:00-13:00", "Tennis", "AE", "CS", getVenueForSport("Tennis")},
        {"Thurs", "2023-04-13", "12:00-13:00", "Basketball", "SS", "MS", getVenueForSport("Basketball")},
        {"Thurs", "2023-04-13", "13:30-14:30", "Tug of War", "EE", "ME", getVenueForSport("Tug of War")},

        // Day 2 – 2023-04-14
        {"Fri", "2023-04-14", "09:00-10:00", "Futsal", "CS", "EE", getVenueForSport("Futsal")},
        {"Fri", "2023-04-14", "09:00-10:00", "Badminton", "MS", "ME", getVenueForSport("Badminton")},
        {"Fri", "2023-04-14", "10:30-11:30", "Basketball", "SS", "AE", getVenueForSport("Basketball")},
        {"Fri", "2023-04-14", "12:00-13:00", "Tennis", "AE", "CS", getVenueForSport("Tennis")},
        {"Fri", "2023-04-14", "12:00-13:00", "Tug of War", "SS", "MS", getVenueForSport("Tug of War")},
        {"Fri", "2023-04-14", "13:30-14:30", "Football", "EE", "ME", getVenueForSport("Football")},

        // Day 3 – 2023-04-15
        {"Sat", "2023-04-15", "09:00-10:00", "Badminton", "CS", "EE", getVenueForSport("Badminton")},
        {"Sat", "2023-04-15", "09:00-10:00", "Basketball", "MS", "ME", getVenueForSport("Basketball")},
        {"Sat", "2023-04-15", "10:30-11:30", "Tennis", "SS", "AE", getVenueForSport("Tennis")},
        {"Sat", "2023-04-15", "12:00-13:00", "Tug of War", "AE", "CS", getVenueForSport("Tug of War")},
        {"Sat", "2023-04-15", "13:30-14:30", "Football", "SS", "MS", getVenueForSport("Football")},
        {"Sat", "2023-04-15", "15:00-16:00", "Futsal", "EE", "ME", getVenueForSport("Futsal")},

        // Day 4 – 2023-04-16
        {"Sun", "2023-04-16", "09:00-10:00", "Basketball", "CS", "EE", getVenueForSport("Basketball")},
        {"Sun", "2023-04-16", "09:00-10:00", "Tennis", "MS", "ME", getVenueForSport("Tennis")},
        {"Sun", "2023-04-16", "10:30-11:30", "Tug of War", "SS", "AE", getVenueForSport("Tug of War")},
        {"Sun", "2023-04-16", "12:00-13:00", "Football", "AE", "CS", getVenueForSport("Football")},
        {"Sun", "2023-04-16", "12:00-13:00", "Futsal", "SS", "MS", getVenueForSport("Futsal")},
        {"Sun", "2023-04-16", "13:30-14:30", "Badminton", "EE", "ME", getVenueForSport("Badminton")},

        // Day 5 – 2023-04-17
        {"Mon", "2023-04-17", "09:00-10:00", "Tennis", "CS", "EE", getVenueForSport("Tennis")},
        {"Mon", "2023-04-17", "09:00-10:00", "Tug of War", "MS", "ME", getVenueForSport("Tug of War")},
        {"Mon", "2023-04-17", "10:30-11:30", "Football", "SS", "AE", getVenueForSport("Football")},
        {"Mon", "2023-04-17", "12:00-13:00", "Futsal", "AE", "CS", getVenueForSport("Futsal")},
        {"Mon", "2023-04-17", "12:00-13:00", "Badminton", "SS", "MS", getVenueForSport("Badminton")},
        {"Mon", "2023-04-17", "13:30-14:30", "Basketball", "EE", "ME", getVenueForSport("Basketball")},

        // Day 6 – 2023-04-18
        {"Tues", "2023-04-18", "09:00-10:00", "Tug of War", "CS", "EE", getVenueForSport("Tug of War")},
        {"Tues", "2023-04-18", "09:00-10:00",  "Basketball", "SS", "MS", getVenueForSport("Basketball")},
        {"Tues", "2023-04-18", "10:30-11:30", "Futsal", "SS", "AE", getVenueForSport("Futsal")},
        {"Tues", "2023-04-18", "12:00-13:00", "Badminton", "AE", "CS", getVenueForSport("Badminton")},
        {"Tues", "2023-04-18", "12:00-13:00", "Football", "MS", "ME", getVenueForSport("Football")},
        {"Tues", "2023-04-18", "13:30-14:30", "Tennis", "EE", "ME", getVenueForSport("Tennis")}
    };         

    saveToFile();
}

    void saveToFile() {
        ofstream out(filename);
        for (int i = 0; i < schedule.size(); ++i) {
            out << schedule[i].toLine() << endl;
        }
        out.close();
    }

    void loadFromFile() {
        ifstream in(filename);
        string line;
        while (getline(in, line)) {
            if (!line.empty())
                schedule.push_back(MatchSlot::fromLine(line));
        }
        in.close();
    }
    
    //to delete a match
    
    void deletematch()
    {
    	viewSchedule();
    	if(schedule.empty()) return;
    	
	
	
        int index;
        cout<<"\nEnter match number to delete" ;
        cin>> index;
        
        if(index<1 || index> schedule.size())
        {
            cout<<"invalid match number"<<endl;
            return;
            
        }
        
        string confirm;
        
        cout<<"are you sure you want to delete match #"<< index<<"? (yes/no):" <<endl;
        cin>> confirm;
        
        if (confirm =="yes")
        {
            schedule.erase(schedule.begin()+ index-1);
            saveToFile();
            
            cout<<"match deleted successfully"<<endl;
            
        }
        else if ( confirm=="no")
        {
            cout<<"deletion cancelled";
        }
        else
        {
            cout<<"invalid input";
            return;
        }
	logAdminAction("Admin deleted match #" + to_string(index));//admin

    }
    
};

class Player : public User{
private:
    string PlayerName;
    int PlayerAge;
    string Gender;
    int KitNo;
    string Dept;
    vector<string> sport;
    static const string sports[6]; //sports list as classlevel constant
    ScheduleManager m;
    
public:
    string TeamName;
    
    //to save players to a file
    void savePlayerToFile(const vector<Player> players, const string& filename){
        ofstream out(filename);
        for(const auto& p : players) {
            out<<p.toLine()<<endl;
        }
        out.close();
    }
    
    //loads player from a file
    vector<Player> loadPlayerFromFile(const string& filename){
        vector<Player> players;
        ifstream in(filename);
        string line;
        while(getline(in,line)){
            if(!line.empty()) {
                players.push_back(Player::fromLine(line));
            }
        }
        in.close();
        return players;
    }


    string getTeamFromDept(const string& d){
        if(d=="CS" || d=="cs") return "CS";
        else if(d=="DS" || d=="ds") return "DS";
        else if(d=="AI" || d=="ai") return "AI";
        else if(d=="MS" || d=="ms") return "MS";
        else if(d=="SS" || d=="ss") return "SS";
        else if(d=="ME" || d=="me") return "ME";
        else if(d=="EE" || d=="ee") return "EE";
        else if(d=="AE" || d=="ae") return "AE";
        return "Unknown";
    }
    

    void getInfo() {
        cout << "\n=========== SPORTSYNC PLAYER REGISTRATION FORM ===========" << endl;

        cout << "Player Registration ID: ";
        while (!(cin >> ID)) {
            cout << "Invalid input. Please enter a valid number: ";
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
        }
        cin.ignore();

        cout << "Player Name: ";
        getline(cin, PlayerName);

        cout << "Player Age: ";
        while (!(cin >> PlayerAge)) {
            cout << "Invalid input. Please enter a valid age: ";
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
        }
        cin.ignore();

        cout << "Player Gender (M/F/Other): ";
        getline(cin, Gender);

        cout << "Player Kit Number: ";
        while (!(cin >> KitNo)) {
            cout << "Invalid input. Please enter a valid number: ";
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
        }
        cin.ignore();

        cout << "Player Department (initials): ";
        getline(cin, Dept);

        cout << "\n------ Available Sports ------\n";
        for (int i = 0; i < 6; ++i) {
            cout << i + 1 << ". " << sports[i] << endl;
        }

        cout<<"\nSelect the number of the sports you want to register in (seperated by spaces, end with 0): ";
        int sportChoice;
        while(cin>> sportChoice && sportChoice !=0){
            if(sportChoice >=1 && sportChoice<=6){
                sport.push_back(sports[sportChoice-1]);
            } 
            else {
                cout<<"\nInvalid choice. Please select from 1 to 9."<<endl;
            }
        }
        cin.ignore();
        TeamName = getTeamFromDept(Dept);
    }
    
    string getTeamName() const {
    return TeamName;
    }   

    int getID() const{
        return ID;
    }

    void login() override{
            int choice;
            cout<<"\nWelcome player----Please choose your choice: "<<endl;
            cout<<"\n1. Register for sports."<<endl;
            cout<<"\n2. Login to view schedule."<<endl;
            cout<<"\n3. Exit"<<endl;
            cout<<"\n------------------"<<endl;
            while (!(cin >> choice)) {
	        cout << "Invalid input. Please enter a number (1–3): ";
	        cin.clear();
	        cin.ignore(1000, '\n');
    }

            switch(choice){
                case 1: {
                getInfo(); //fills current object
                 vector<Player> allPlayers = loadPlayerFromFile("players.txt");
                allPlayers.push_back(*this); //adds this player
                savePlayerToFile(allPlayers, "players.txt"); //update file
                cout<<"\nRegistered Successfully!!"<<endl;
                ShowInfo();
                break;
                }
                case 2: {
                int loginID;
                cout<<"\nPlease enter your ID to login: "<<endl;
                cin>>loginID;

                vector<Player> allPlayers = loadPlayerFromFile("players.txt");
                bool found = false;

                for(const auto& p: allPlayers){
                    if(p.ID == loginID) {
                        cout<<"\nLogin successful.."<<endl;
                        m.viewMySchedule(p);
                        found = true;
                        break;
                    }
                }
                if(!found){
                    cout<<"\nNo player found with this ID."<<endl;
                }
                break;
                }
                case 3:
                    return;
                default:
                cout<<"\nInvalid choice."<<endl;
            }
        }

    void ShowInfo() const {
        cout << "\n========= REGISTERED PLAYER DETAILS =========" << endl;
        cout << "Player ID         : " << ID << endl;
        cout << "Name              : " << PlayerName << endl;
        cout << "Age               : " << PlayerAge << endl;
        cout << "Gender            : " << Gender << endl;
        cout << "Kit Number        : " << KitNo << endl;
        cout << "Department        : " << Dept << endl;
        cout << "Registered Sport  : ";
        for(const auto& s : sport) cout<< s << " "<<endl;
        cout << "=============================================" << endl;
    }

    //converts player info to a single line of text
    string toLine() const {
    string line = to_string(ID) +" , "+PlayerName + " , " + to_string(PlayerAge) + " , " 
    + Gender + " , " + to_string(KitNo) + " , " + Dept + " , ";

    for(size_t i = 0; i<sport.size();i++){
        line +=sport[i];
        if(i != sport.size()-1) { line+=";";} //separate sports with ;
    }
    return line;
    }

    //to load a player from a line
    static Player fromLine(const string& line) {
        Player p;
        stringstream ss(line);
        string temp,sportsStr;

        getline(ss, temp, ','); p.ID = stoi(temp);
        getline(ss, p.PlayerName, ',');
        getline(ss, temp, ','); p.PlayerAge = stoi(temp);
        getline(ss, p.Gender, ',');
        getline(ss, temp, ','); p.KitNo = stoi(temp);
        getline(ss, p.Dept, ',');
        getline(ss, sportsStr, ',');

        stringstream sportSS(sportsStr);
        while(getline(sportSS,temp,';')){
            p.sport.push_back(temp);
        }

        // Optional: Set TeamName from Dept
        p.TeamName = p.getTeamFromDept(p.Dept);
        return p;
    }

};

// Definition of static array outside class
const string Player::sports[6] = {
    "Football", "Badminton", "Futsal", "BasketBall", "Tug Of War", "Tennis"
};

void ScheduleManager::viewMySchedule(const Player& p) {
    string team = p.getTeamName();  // use getter
    cout << "\nYour Match Schedule:\n";
    cout << "No | Day       | Date       | Time        | Event        | Match         | Venue\n";
    cout << "-------------------------------------------------------------------------------------------\n";

    bool found = false;
    for (int i = 0; i < schedule.size(); ++i) {
        if (schedule[i].team1 == team || schedule[i].team2 == team) {
            schedule[i].display(i);
            found = true;
        }
    }
    if (!found) {
        cout << "\nNo matches scheduled for your team.\n";
    }
}


class admin : public User
{
	private:
	ScheduleManager manage;	
	string adminpass="pass123";
	string  systempass="0000";
			
	public:
		admin(){
            ID = 1234567890;
        }
		void login() override
		{
			int attempts=0;
			string choice;
            int IDchoice;
				   
			    while( attempts<3)
				{
                
                //asking for admin ID
                cout<<"\nEnter admin ID: "<<endl;
                 while (!(cin >> IDchoice)) {
	            cout << "Invalid input. Please enter numbers only: ";
	            cin.clear();
	            cin.ignore(1000, '\n');
        }

                while(IDchoice != ID){
                cout<<"\nInvalid admin ID \nPlease try again----"<<endl;
                  while (!(cin >> IDchoice)) {
	              while (!(cin >> IDchoice)) {
                  cout << "Invalid input. Please enter numbers only: ";
                   cin.clear();
                   cin.ignore(1000, '\n');
        }

        }

                }
                

                //Asking for admin password
				cout<<"\nEnter admin password: ";
				cin>>choice;
			
				if(choice==adminpass)
				{
				   	cout<<"\nAccess granted!!"<<endl;
				   	cout<<"\nYou are now logged in as an admin."<<endl;
				   		
				   		
				   	while(true)
				   	{
				   		int opt;
				   		cout<<"\n>>>>>>>>>>ADMIN MENU<<<<<<<<<<"<<endl;;
				   		cout << "\n1. View Schedule."<<endl;
		                cout << "\n2. Add Match."<<endl;
		                cout << "\n3. Edit Match."<<endl;
		                cout << "\n4. Delete Match."<<endl;
		                cout << "\n5. Exit"<<endl;
                        cout<<"\n----------------------"<<endl;
		                cout << "\nEnter option: ";
		                
		                 while (!(cin >> opt)) {
                    cout << "Invalid input. Please enter a number (1-5): ";
                    cin.clear();
                    cin.ignore(1000, '\n');
                }
                
		                if(opt == 1)
	                    {
	                        manage.viewSchedule();
						}
						else if(opt == 2)
						{
							manage.addMatch();
						}
						else if(opt == 3)
						{
							manage.editMatch();
						}
				   		else if(opt == 4)
				   		{
				   			manage.deletematch();
						}
						else if(opt == 5)
						{
							cout<<"\nLogging out. . . .  ."<<endl;
							return;
						}
						else
						{
							cout<<"\nInvalid option. Try again.."<<endl;
						}
					}
				}
				else
				{
					attempts++;
					cout<<"\nIncorrect passcode!!"<<endl;
					cout<<"\nAttempts left: "<<3-attempts<<endl;
				}
				
            }
			       
			    cout<<"\nToo many failed attempts!!"<<endl;
                cout<<"\n______________________________"<<endl;
			    cout<<"\nEnter system passcode: ";
		        cin>>choice;
			       
			       
			   if(choice == systempass)
		       {
		       	cout<<"\nSystem pass granted!"<<endl;
		       	cout<<"\nLogging in as admin...."<<endl;
			       	
			       	
				while(true)
				   	{
				   		int opt;
				   		cout<<"\n>>>>>>>>>>ADMIN MENU<<<<<<<<<<"<<endl;;
				   		cout << "\n1. View the Schedule."<<endl;
		                cout << "\n2. Add a Match."<<endl;
		                cout << "\n3. Edit a Match."<<endl;
		                cout << "\n4. Delete a Match."<<endl;
		                cout << "\n5. Exit"<<endl;
                        cout<<"\n----------------------"<<endl;
		                cout << "\nEnter option: ";
		               while (!(cin >> opt)) {
                    cout << "Invalid input. Please enter a number (1–5): ";
                    cin.clear();
                    cin.ignore(1000, '\n');
                }
		                
		                if(opt == 1)
	                    {
	                        manage.viewSchedule();
						}
						else if(opt == 2)
						{
							manage.addMatch();
						}
						else if(opt == 3)
						{
							manage.editMatch();
						}
				   		else if(opt == 4)
				   		{
				   			manage.deletematch();
						}
						else if(opt == 5)
						{
							cout<<"\nLogging out. . . .  ."<<endl;
							return;
						}
						else
						{
							cout<<"\nInvalid option. Try again.."<<endl;
						}
	
					}    
			   }
				   
			else
			{
    		   	cout<<"\nWrong system password.\n Contact IT department.\n Sytem shutting down . . . :"<<endl;
			   	return;
		    }
        }   
};

class Viewer : public User {
    private:
    ScheduleManager manage;
    public:
    void login() override 
    {
		int choice;
		while (true)
		{
			cout << "\n========= VIEWER MODE =========" << endl;
            cout << "1. View Schedule" << endl;
            cout << "2. Exit" << endl;
            cout << "Enter your choice: ";
         
                while (!(cin >> choice)) {
                cout << "Invalid input. Please enter 1 or 2: ";
                cin.clear();
                cin.ignore(1000, '\n');
            }
         
            switch (choice){
                case 1:
					manage.viewSchedule();
				break;
        		
                case 2:
            		cout << "Goodbye!\n";
                return;
		
                default:
                   cout << "Invalid option.Please enter 1 or 2\n";
                break;
			}
        }
    }
};

//main class that relates all the classes
//also for abstraction -> data hiding
class classManager{
    private:
    admin a;
    Player p;
    Viewer v;
    public:
    void menu()
    {
        int user;
        User* u=nullptr;
        do {
        cout<<"\n===============>>WELCOME TO SPORTSYNC<<==============="<<endl;
        cout<<"\nWhat would you like to login as?"<<endl;
        cout<<"\n1.Admin"<<endl;
        cout<<"\n2.Viewer"<<endl;
        cout<<"\n3.Player"<<endl;
        cout<<"\n4.Exit"<<endl;
        cout<<"\n-----------------------"<<endl;
        cout<<"Enter choice: ";
      
                 while (!(cin >> user)) {
                cout << "Invalid input. Please enter a number between 1 and 4: ";
                cin.clear();
                cin.ignore(1000, '\n');
            }
            
        switch(user){
            case 1:
               u = &a;
            break;

            case 2:
               u = &v;
            break;

            case 3:
                u = &p;
                
            break;

            case 4:
            cout<<"\nExiting SportSync..."<<endl;
            return;

            default:
            cout<<"\nInvalid option!! Please try again..\n";
            continue;
            }
            //calling the overridden login function thru base pointer
            if(u) 
            {
                u->login();
            }
        }
        while(true);
    }
};

int main() {
   classManager MAIN;
   MAIN.menu(); 
    return 0;
}
