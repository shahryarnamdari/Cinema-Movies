#include <iostream>
#include <sstream>
#include <algorithm>
#include <fstream>
#include <vector>
#include <string>

using namespace std;

typedef struct Movie { //struct for each date
	string CinemaName;//ColumnNum[0]
	string MovieName;//ColumnNum[1]
	string Price;//ColumnNum[5]
	string Day;//ColumnNum[2]
	string StartingTime;//ColumnNum[3]
	string FinishingTime;//ColumnNum[4]
	string Hall;//ColumnNum[6]
}Movie;
void ReadFile(ifstream& myfile, int ColumnNum[], vector<Movie>& MOVIES,char* argv[])
{
	myfile.open(argv[1]);//the second element of argv(argv[1]) is our file address
	string line;
	string token;
	string delim = ",";
	getline(myfile, line);
	while (getline(myfile, line))
	{
		Movie NewMovie;
		auto start = 0U;
		auto end = line.find(delim);
		for (int i = 0;i < 7;i++)
		{
			token = line.substr(start, end - start);
			//pushbacks
			if (ColumnNum[i] == 0)
				NewMovie.CinemaName=token;
			else if (ColumnNum[i] == 1)
				NewMovie.MovieName = token;
			else if (ColumnNum[i] == 2)
				NewMovie.Day = token;
			else if (ColumnNum[i] == 3)
				NewMovie.StartingTime= token;
			else if (ColumnNum[i] == 4)
				NewMovie.FinishingTime = token;
			else if (ColumnNum[i] == 5)
				NewMovie.Price = token;
			else if (ColumnNum[i] == 6)
				NewMovie.Hall = token;
			//preparing for next step
			start = end + delim.length();
			end = line.find(delim, start);
		}
		MOVIES.push_back(NewMovie);//add new movie to the vector of movies
	}
	myfile.close();
}
void Locate(ifstream& myfile, int ColumnNum[], char* argv[])
{
	myfile.open(argv[1]);//the second element of argv(argv[1]) is our file address
	string line;
	getline(myfile, line);
	string token;
	string delim = ",";
	auto start = 0U;
	auto end = line.find(delim);
	for (int i = 0;i < 7;i++)
	{
		token = line.substr(start, end - start);
		//locating the column
		if (!token.compare("CinemaName"))
			ColumnNum[i] = 0;
		else if (!token.compare("MovieName"))
			ColumnNum[i] = 1;
		else if (!token.compare("Day"))
			ColumnNum[i] = 2;
		else if (!token.compare("StartingTime"))
			ColumnNum[i] = 3;
		else if (!token.compare("FinishingTime"))
			ColumnNum[i] = 4;
		else if (!token.compare("Price"))
			ColumnNum[i] = 5;
		else if (!token.compare("Hall"))
			ColumnNum[i] = 6;
		//preparing for next step
		start = end + delim.length();
		end = line.find(delim, start);
	}
	myfile.close();
}
bool Mycompare(string a, string b) {return a<b;}
void AlphabaticallySort(vector<string> &list) {
	sort(list.begin(), list.end(), Mycompare);
}
void PrintMovieNames(vector<Movie>& MOVIES)
{
	if (MOVIES.size() > 0)//if there was a movie
	{
		vector<string> MovieName;//make list of all movie names
		for (int i = 0;i < MOVIES.size();i++)
			MovieName.push_back(MOVIES[i].MovieName);

		AlphabaticallySort(MovieName);//sorting alphabatically
		cout << MovieName[0];//printing the first movie
		for (int i = 1;i < MovieName.size();i++)
			if (MovieName[i] != MovieName[i - 1])
				cout << MovieName[i] << endl;
	}
}
int checkErrorAndFindMovieName(string &MovieName,string cmd, vector<Movie>& MOVIES)
{
	//0 means error
	//1 means GET ALL MOVIES
	//2 means GET SCHEDULE <MOVIE NAME>

	vector<string> token;
	string delim = " ";
	size_t pos = 0;
	do
	{
		pos = cmd.find(delim);
		token.push_back(cmd.substr(0, pos));//saving all words od cmd in token vector
		cmd.erase(0, pos + delim.length());//preparing for next step
	}while (pos != string::npos);
	//check errors
	if (token.size() < 3)
		return 0;
	if (!token[0].compare("GET")&!token[1].compare("ALL")&!token[2].compare("MOVIES")& token.size() == 3)
		return 1;
	else if (!token[0].compare("GET")&!token[1].compare("SCHEDULE")&token.size() > 2)
	{
		for (int i = 3;i < token.size();i++)
			token[2] = token[2] + " " + token[i];//concat a movie name
		for (int i = 0;i < MOVIES.size();i++)
			if (MOVIES[i].MovieName == token[2])
			{
				MovieName = token[2];
				return 2;
			}
	}
	else
		return 0;
}
bool Mycompare2(const Movie& MOVIES1, const Movie& MOVIES2){ return MOVIES1.CinemaName < MOVIES2.CinemaName;}
void cinemaNameSort(vector<Movie>& MOVIES){sort(MOVIES.begin(), MOVIES.end(), Mycompare2);}
bool Mycompare3(const Movie& MOVIES1, const Movie& MOVIES2)
{ 
	if (MOVIES1.Day == "Sunday") {
		if (MOVIES2.Day == "Saturday")
			return 0;
	}
	else if (MOVIES1.Day == "Monday") {
		if (MOVIES2.Day == "Saturday" || MOVIES2.Day == "Sunday")
			return 0;
	}
	else if (MOVIES1.Day == "Tuesday") {
		if (MOVIES2.Day == "Saturday" || MOVIES2.Day == "Sunday" || MOVIES2.Day == "Monday")
			return 0;
	}
	else if (MOVIES1.Day == "Wednesday") {
		if (MOVIES2.Day == "Saturday" || MOVIES2.Day == "Sunday" || MOVIES2.Day == "Monday" || MOVIES2.Day == "Tuesday")
			return 0;
	}
	else if (MOVIES1.Day == "Thursday") {
		if (MOVIES2.Day == "Saturday" || MOVIES2.Day == "Sunday" || MOVIES2.Day == "Monday" || MOVIES2.Day == "Tuesday" || MOVIES2.Day == "Wednesday")
			return 0;
	}
	else if (MOVIES1.Day == "Friday") {
		if(MOVIES2.Day != "Friday")
			return 0;
	}
	if (MOVIES1.Day == MOVIES2.Day)
	{
		if (MOVIES2.StartingTime < MOVIES1.StartingTime|| MOVIES2.StartingTime == MOVIES1.StartingTime)
			return 0;
	}
	return 1;
}
void startTimeSort(vector<Movie>& MOVIES)
{
	int j;
	Movie key;
	int size = MOVIES.size();
	for (int i = 1; i<size; i++) {
		key = MOVIES[i];//take value
		j = i;
		while (true)
		{
			if (j <= 0)
				break;
			if (!Mycompare3(key, MOVIES[j - 1]))
				break;
			MOVIES[j] = MOVIES[j - 1];
			j--;
		}
		MOVIES[j] = key;//insert in right place
	}
}
vector<Movie> chooseMoviesToDisplay(string MovieName, const vector<Movie>& MOVIES)
{
	//choose Movies to display
	vector<Movie> ChosenMovies;//making array of Movies struct

	for (int i = 0;i < MOVIES.size(); i++)
		if (MOVIES[i].MovieName == MovieName)
		{
			ChosenMovies.push_back(MOVIES[i]);
			break;
		}

	for (int i = 0;i < MOVIES.size(); i++)
	{
		if (MOVIES[i].MovieName == MovieName)
		{
			//if we get interference between MOVIES[i] and last chosen movie
			if (MOVIES[i].Day == ChosenMovies[ChosenMovies.size() - 1].Day & MOVIES[i].StartingTime < ChosenMovies[ChosenMovies.size() - 1].FinishingTime & ChosenMovies[ChosenMovies.size() - 1].StartingTime < MOVIES[i].FinishingTime)
				continue;
			//else if(MOVIES[i].StartingTime = ChosenMovies[ChosenMovies.size() - 1].FinishingTime & ChosenMovies[ChosenMovies.size() - 1].StartingTime < MOVIES[i].FinishingTime)
			else
				ChosenMovies.push_back(MOVIES[i]);
		}
	}
	return ChosenMovies;
}
void priceSort(vector<Movie>& MOVIES) {
	int j;
	Movie key;
	int size =MOVIES.size();
	for (int i = 1; i<size; i++) {
		key = MOVIES[i];//take value
		j = i;
		while (true)
		{
			if (j <= 0)
				break;
			if (MOVIES[j - 1].Price < key.Price|| MOVIES[j - 1].Price == key.Price)
				break;
			MOVIES[j] = MOVIES[j - 1];
			j--;
		}
		MOVIES[j] = key;//insert in right place
	}
}
int movieTimeLengthCalc(const Movie& movie)
{
	//Calculating Movie Time Length
	int HoursLength = stoi(movie.FinishingTime.substr(0, 2)) - stoi(movie.StartingTime.substr(0, 2));
	int MinutesLength = stoi(movie.FinishingTime.substr(3, 5)) - stoi(movie.StartingTime.substr(3, 5));
	int MovieTimeLength = HoursLength * 2 * 30 + MinutesLength;//in minutes
	return MovieTimeLength;
}
void printDayLine(string DayName, const vector<Movie>& ChosenMovies)
{
	cout << DayName;
	int PrintedCharCounter = DayName.size();
	int LastChar = PrintedCharCounter;
	for (int i = 0;i < ChosenMovies.size();i++)
	{
		if (ChosenMovies[i].Day == DayName)
		{
			int Hours = stoi(ChosenMovies[i].StartingTime.substr(0, 2));
			int Minutes = stoi(ChosenMovies[i].StartingTime.substr(3, 5));
			int NumOfSpaces = (Hours * 2 * 30 + Minutes - 8 * 2 * 30) / 30 * 5 + 10 - LastChar;
			for (int j = 0;j < NumOfSpaces;j++)
				cout << ' ';
			cout << '|' << ChosenMovies[i].CinemaName;
			PrintedCharCounter += (NumOfSpaces + 1 + ChosenMovies[i].CinemaName.size());
			//Calculating Movie Time Length
			int MovieTimeLength = movieTimeLengthCalc(ChosenMovies[i]);
			int NumOfSpacesAfterCinemaName = MovieTimeLength / 30 * 5 - 1 - ChosenMovies[i].CinemaName.size();
			for (int j = 0;j <NumOfSpacesAfterCinemaName;j++) {
				cout << ' ';
				PrintedCharCounter++;
			}
			if (i + 1 == ChosenMovies.size())
			{
				cout << '|';
				PrintedCharCounter++;
			}
			else if (ChosenMovies[i + 1].StartingTime == ChosenMovies[i].FinishingTime&ChosenMovies[i + 1].Day == ChosenMovies[i].Day) {
				LastChar = PrintedCharCounter;
				continue;
			}
			else {
				cout << '|';
				PrintedCharCounter++;
			}
		}
		LastChar = PrintedCharCounter;
	}
	for (int i = 0;i < 175 - PrintedCharCounter;i++)
		cout << ' ';
	cout << endl;
}
bool Mycompare4(const Movie& MOVIES1, const Movie& MOVIES2) { return MOVIES1.StartingTime < MOVIES2.StartingTime; }
void startHourSort(vector<Movie>&TwoDaysMovies) { sort(TwoDaysMovies.begin(), TwoDaysMovies.end(), Mycompare4); }
void setStartEndCol(vector<int>& StartEndCol, const vector<Movie>& TwoDaysMovies)
{
	for (int i = 0;i < TwoDaysMovies.size();i++)
	{
		//start col
		int Hours = stoi(TwoDaysMovies[i].StartingTime.substr(0, 2));
		int Minutes = stoi(TwoDaysMovies[i].StartingTime.substr(3, 5));
		int Column = (Hours * 2 * 30 + Minutes - 8 * 2 * 30) / 30 * 5 + 10;
		StartEndCol.push_back(Column);
		//end col
		Hours = stoi(TwoDaysMovies[i].FinishingTime.substr(0, 2));
		Minutes = stoi(TwoDaysMovies[i].FinishingTime.substr(3, 5));
		Column = (Hours * 2 * 30 + Minutes - 8 * 2 * 30) / 30 * 5 + 10;
		StartEndCol.push_back(Column);
	}
}
void printMiddleLine(string DayName1, string DayName2, const vector<Movie>& ChosenMovies)
{
	vector<Movie> TwoDaysMovies;
	for (int i = 0;i < ChosenMovies.size();i++)
		if (ChosenMovies[i].Day == DayName1 || ChosenMovies[i].Day == DayName2)
			TwoDaysMovies.push_back(ChosenMovies[i]);

	startHourSort(TwoDaysMovies);//sort on start Hour

	vector<int> StartEndCol;
	setStartEndCol(StartEndCol, TwoDaysMovies);

	for (int i = 0;i < 175;i++)
	{
		int flag = 0;
		for (int j = 0;j < StartEndCol.size();j++)
		{
			if (i == StartEndCol[j]) 
			{
				cout << '+';
				flag = 1;
				break;
			}
		}
		if (flag == 1)
			continue;
		for (int j = 0;j < StartEndCol.size();j+=2)
		{
			if (i > StartEndCol[j]&i<StartEndCol[j+1])
			{
				cout << '-';
				flag = 1;
				break;
			}
		}
		if (flag == 1)
			continue;
		cout << ' ';
	}
	cout << endl;
}
void PtintSchedule(const vector<Movie>& ChosenMovies)
{

	////////////////////////printing first constant line
	string space15 = "               ";//15 spaces
	cout << "          " << "08:00" << space15 << "10:00";
	cout << space15 << "12:00" << space15 << "14:00" << space15;
	cout << "16:00" << space15 << "18:00" << space15 << "20:00";
	cout << space15 << "22:00" << space15 << "00:00" << endl;

	///Bsaturday
	printMiddleLine("empty","Saturday", ChosenMovies);
	///saturday
	printDayLine("Saturday", ChosenMovies);
	///Bsunday
	printMiddleLine("Saturday", "Sunday", ChosenMovies);
	///sunday
	printDayLine("Sunday", ChosenMovies);
	///Bmonday
	printMiddleLine("Sunday","Monday", ChosenMovies);
	///monday
	printDayLine("Monday", ChosenMovies);
	///Btuesday
	printMiddleLine("Monday", "Tuesday", ChosenMovies);
	///tuesday
	printDayLine("Tuesday", ChosenMovies);
	///Bwednesday
	printMiddleLine("Tuesday", "Wednesday", ChosenMovies);
	///wednesday
	printDayLine("Wednesday", ChosenMovies);
	///Bthursday
	printMiddleLine("Wednesday", "Thursday", ChosenMovies);
	///thursday
	printDayLine("Thursday", ChosenMovies);
	///Bfriday
	printMiddleLine("Thursday", "Friday", ChosenMovies);
	///friday
	printDayLine("Friday", ChosenMovies);
	///last line
	printMiddleLine("Friday","empty", ChosenMovies);
}
void getScheduleTerminal(string MovieName, vector<Movie> MOVIES)
{
	//Sort MOVIES
	cinemaNameSort(MOVIES);
	priceSort(MOVIES);
	startTimeSort(MOVIES);

	//choose movies we want to display
	vector<Movie> ChosenMovies = chooseMoviesToDisplay(MovieName, MOVIES);

	//print
	PtintSchedule(ChosenMovies);
}
void printDefaultParts(ofstream &HTMLFile)
{
	HTMLFile << "<html lang=\"en\"> "<<endl << "<head>" << endl;
	HTMLFile << "<title></title>" << endl << "<link rel=\"stylesheet\" href=\"./style.css\" />" << endl;
	HTMLFile << "</head>" << endl << "<body>" << endl;
	//time-box
	int Left = 100;
	string Hour = "08";
	for (int i = 0;i < 8;i++)
	{
		HTMLFile << "<div class=\"time-box\" style=\"left: " << Left << "px;\"><p>" << Hour << ":00</p></div>"<<endl;
		Left += 200;
		Hour = to_string(stoi(Hour) + 2);
	}
	HTMLFile << "<div class=\"time-box\" style=\"left: " << 1700 << "px;\"><p>" << "00" << ":00</p></div>" << endl;
	//vertical-line
	HTMLFile << endl;
	Left = 100;
	for (int i = 0;i < 17;i++)
	{
		HTMLFile << "<div class=\"vertical-line\" style=\"left: "<< Left <<"px;\"></div>"<<endl;
		Left += 100;
	}
	//day-box
	HTMLFile << endl;
	int top = 60;
	vector<string> Days = { "Saturday","Sunday","Monday","Tuesday","Wednesday","Thursday","Friday" };
	for (int i = 0;i < 7;i++)
	{
		HTMLFile << "<div class=\"day-box\" style=\"top: "<<top<<"px;\">"<<Days[i]<<"</div>"<<endl;
		top += 50;
	}
}
int findWidth(const Movie &ChosenMovie)
{
	int Width;
	Width = 50 * (movieTimeLengthCalc(ChosenMovie)/30);
	return Width;
}
int findLeft(const Movie &ChosenMovie)
{
	int Left;
	int Hours = stoi(ChosenMovie.StartingTime.substr(0, 2));
	int Minutes = stoi(ChosenMovie.StartingTime.substr(3, 5));
	Left = 100+50 * (Hours*2*30+Minutes-480)/30;
	return Left;
}
int findTop(const Movie &ChosenMovie)
{
	int Top,dayNum;
	if (ChosenMovie.Day == "Saturday")
		dayNum = 0;
	else if (ChosenMovie.Day == "Sunday")
		dayNum = 1;
	else if (ChosenMovie.Day == "Monday")
		dayNum = 2;
	else if (ChosenMovie.Day == "Tuesday")
		dayNum = 3;
	else if (ChosenMovie.Day == "Wednesday")
		dayNum = 4;
	else if (ChosenMovie.Day == "Thursday")
		dayNum = 5;
	else if (ChosenMovie.Day == "Friday")
		dayNum = 6;
	Top = 60 + dayNum * 50;
	return Top;
}
void makehtmlFile(string MovieName, vector<Movie> MOVIES)
{
	//Sort MOVIES
	cinemaNameSort(MOVIES);
	priceSort(MOVIES);
	startTimeSort(MOVIES);

	//choose movies we want to display
	vector<Movie> ChosenMovies = chooseMoviesToDisplay(MovieName, MOVIES);

	//make <MovieName>.html
	ofstream HTMLFile(MovieName+".html");
	printDefaultParts(HTMLFile);

	//record-box
	//Sort MOVIES
	cinemaNameSort(MOVIES);
	priceSort(MOVIES);
	startTimeSort(MOVIES);

	//record-box
	HTMLFile << endl;
	for (int i = 0;i < ChosenMovies.size();i++)
	{
		int Width=findWidth(ChosenMovies[i]);
		int Left= findLeft(ChosenMovies[i]);
		int Top = findTop(ChosenMovies[i]);
		HTMLFile << "<div class=\"record-box\" style=\"width: "<<Width<<"px; left: "<<Left<<"px; top:"<<Top<<"px; \">"<<ChosenMovies[i].CinemaName<<"</div>" << endl;
	}
	HTMLFile << endl << "</body>" << endl << "</html>" << endl;
	HTMLFile.close();
}
string getCommandLine()
{
	string CommandLine;
	getline(cin, CommandLine);
	istringstream stream(CommandLine);
	return CommandLine;
}
int main(int argc,char* argv[])
{
	///defining vectors
	int ColumnNum[7];
	vector<Movie> MOVIES;//making array of Movies

	///read file
	ifstream myfile;
	Locate(myfile, ColumnNum,argv);//for locating titles column
	ReadFile(myfile, ColumnNum, MOVIES, argv);
	///process
	while (true)//infinit loop
	{
		string cmd = getCommandLine();
		string MovieName;
		int check = checkErrorAndFindMovieName(MovieName, cmd, MOVIES);
		if (check == 1)//get all movie names
			PrintMovieNames(MOVIES);
		else if (check == 2)//GET SCHEDULE <MOVIE NAME>    ==>  1.terminal      2.html
		{
			getScheduleTerminal(MovieName, MOVIES);
			makehtmlFile(MovieName, MOVIES);
		}
	}
	return 0;
}
