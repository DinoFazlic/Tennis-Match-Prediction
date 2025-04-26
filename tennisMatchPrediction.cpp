#include <iostream>
#include <vector>
using namespace std;

double gemWinProbability(double p, int rez1, int rez2) {
    int maxPoints = 50;
    vector<vector<double>> dp(maxPoints, vector<double>(maxPoints, 0.0));

    // Setting finishing results
    for (int i = 4; i < maxPoints; i++) {
        for (int j = 0; j < i - 1; j++) {
            if (i >= 4 && i >= j + 2) {
                dp[i][j] = 1.0; // Player 1 wins the gem
            }
            if (j >= 4 && j >= i + 2) {
                dp[i][j] = 0.0; // Player 2 wins the gem
            }
        }
    }

    // Calculating the dp matrix backwords
    for (int i = maxPoints - 1; i >= 0; i--) {
        for (int j = maxPoints - 1; j >= 0; j--) {

            if ((i >= 4 && i >= j + 2) || (j >= 4 && j >= i + 2))
                continue;

            double win1;
            if( i + 1 < maxPoints)
                win1 = dp[i + 1][j];
            else
                win1 = 0.0;

            double win2;
            if( j + 1 < maxPoints)
                win2 = dp[i][j + 1];
            else
                win2 = 0.0;


            dp[i][j] = p * win1 + (1 - p) * win2;
        }
    }

    return dp[rez1][rez2];
}



double tieBreakWinProbability(double p1, double p2, bool firstServerIsPlayer1) {
    int maxPoints = 50;
    vector<vector<double>> dp(maxPoints, vector<double>(maxPoints, 0.0));

    // Setting finishing results
    for (int i = 7; i < maxPoints; i++) {
        for (int j = 0; j < i - 1; j++) {
            if (i >= 7 && i >= j + 2)
                dp[i][j] = 1.0; // Player 1 wins the tie break

            if (j >= 7 && j >= i + 2)
                dp[i][j] = 0.0; // Player 2 wins the tie break

        }
    }

    // Calculating the dp matrix backwords
    for (int i = maxPoints - 1; i >= 0; i--) {
        for (int j = maxPoints - 1; j >= 0; j--) {

            if ((i >= 7 && i >= j + 2) || (j >= 7 && j >= i + 2))
                continue;

            double pCurrent;
            int totalPoints = i + j;

            if (totalPoints == 0) {
                pCurrent = firstServerIsPlayer1 ? p1 : (1 - p2);
            } else if ((totalPoints - 1) % 4 < 2) {
                pCurrent = firstServerIsPlayer1 ? (1 - p2) : p1;
            } else {
                pCurrent = firstServerIsPlayer1 ? p1 : (1 - p2);
            }


            double win1;
            if( i + 1 < maxPoints)
                win1 = dp[i + 1][j];
            else
                win1 = 0.0;

            double win2;
            if( j + 1 < maxPoints)
                win2 = dp[i][j + 1];
            else
                win2 = 0.0;

            dp[i][j] = pCurrent * win1 + (1 - pCurrent) * win2;
        }
    }

    return dp[0][0];
}

bool isPlayer1ServingThisGame(int i, int j, bool firstServerIsPlayer1){
    int currentGameIndex = i + j + 1;

    if (firstServerIsPlayer1)
        return (currentGameIndex % 2 != 0);
    else
        return (currentGameIndex % 2 == 0);

}

double setWinProbability(double p1, double p2, bool firstServerIsPlayer1, int s1, int s2, int g1, int g2){
    const int maxGames = 50;
    vector<vector<double>> dp(maxGames + 1, vector<double>(maxGames + 1, 0.0));

    // Setting finishing results
    for (int i = 0; i <= maxGames; i++) {
        for (int j = 0; j <= maxGames; j++) {
            // If either player has at least 6 games and is ahead by 2, set is decided
            if ((i >= 6 || j >= 6) && abs(i - j) >= 2) {
                dp[i][j] = (i > j) ? 1.0 : 0.0;
            }
            // If the result is 6-6, the tie break decides the set
            else if (i == 6 && j == 6) {
                bool tbFirstServer = isPlayer1ServingThisGame(i, j, firstServerIsPlayer1);
                dp[i][j] = tieBreakWinProbability(p1, p2, tbFirstServer);
            }
        }
    }


    for (int i = maxGames; i >= 0; i--) {
        for (int j = maxGames; j >= 0; j--) {

            bool setIsDecided = ((i >= 6 || j >= 6) && abs(i - j) >= 2);
            bool tieBreakState = (i == 6 && j == 6);

            if (setIsDecided || tieBreakState)
                continue;


            bool isPartialSet = (i == s1 && j == s2);
            bool player1Serving = isPlayer1ServingThisGame(i, j, firstServerIsPlayer1);
            double pWinGame;

            if (player1Serving) {

                if (isPartialSet) {
                    pWinGame = gemWinProbability(p1, g1, g2);
                } else {
                    pWinGame = gemWinProbability(p1, 0, 0);
                }

            } else {

                if (isPartialSet) {
                    double pServerWins = gemWinProbability(p2, g2, g1);
                    pWinGame = 1.0 - pServerWins;
                } else {
                    double pServerWins = gemWinProbability(p2, 0, 0);
                    pWinGame = 1.0 - pServerWins;
                }

            }

            double probIfWin;
            if( i + 1 <= maxGames)
                probIfWin = dp[i + 1][j];
            else
                probIfWin = 1.0;


            double probIfLose;
            if( j + 1 <= maxGames)
                probIfLose = dp[i][j + 1];
            else
                probIfLose = 0.0;

            dp[i][j] = pWinGame * probIfWin + (1.0 - pWinGame) * probIfLose;
        }
    }


    return dp[s1][s2];
}



double matchWinProbability(double p1, double p2, int rezSet1, int rezSet2, int setRez1, int setRez2, int gemRez1, int gemRez2, bool firstServerIsPlayer1){
    const int maxSet = 3;

    vector<vector<double>> dp(maxSet, vector<double>(maxSet, 0.0));

    // Setting finishing results
    for (int i = 0; i < maxSet; i++) {
        for (int j = 0; j < maxSet; j++) {
            if (i == 2) {
                dp[i][j] = 1.0;
            } else if (j == 2) {
                dp[i][j] = 0.0;
            }
        }
    }

    for (int i = maxSet - 1; i >= 0; i--) {
        for (int j = maxSet - 1; j >= 0; j--) {

            if (i == 2 || j == 2)
                continue;

            bool isPartialSet = (i == rezSet1 && j == rezSet2);

            double pSet;
            if (isPartialSet){

                bool firstServerForThisSet;

                if ((setRez1 + setRez2) % 2 != 0)
                    firstServerForThisSet = !firstServerIsPlayer1;
                else
                    firstServerForThisSet = firstServerIsPlayer1;


                pSet = setWinProbability(p1, p2, firstServerForThisSet, setRez1, setRez2, gemRez1, gemRez2);
            }
            else {
                bool firstServerForNextSet;

                if (((i + j) % 2) != 0) {
                    firstServerForNextSet = !firstServerIsPlayer1;
                } else {
                    firstServerForNextSet = firstServerIsPlayer1;
                }

                 pSet = setWinProbability(p1, p2, firstServerForNextSet, 0, 0, 0, 0);
            }


            double probIfP1WinSet  = dp[i + 1][j];
            double probIfP2WinSet  = dp[i][j + 1];


            dp[i][j] = pSet * probIfP1WinSet + (1.0 - pSet) * probIfP2WinSet;
        }
    }

    return dp[rezSet1][rezSet2];
}


int main() {

    double p1, p2;
    cout << "Enter the probability that player 1 wins a point on his serve (p1): ";  
    cin >> p1;
    cout << "Enter the probability that player 2 wins a point on his serve (p2): "; 
    cin >> p2;

    int resultSet1,resultSet2;
    cout<<"Enter the number of sets won (player 1 and player 2):"<<endl; 
    cin>>resultSet1>>resultSet2;

    int currentSet1,currentSet2;
    cout<<"Enter the current set result: "<<endl;
    cin>>currentSet1>>currentSet2;

    int resultGem1,resultGem2;
    cout<<"Enter the current gem result: "<<endl;
    cin>>resultGem1>>resultGem2;

    int player;
    cout<<"Enter the player who serves first (1 or 2): "<<endl; 
    cin>>player;

    bool serve;
    if(player == 1)
        serve = true;
    else
        serve = false;


    double result = matchWinProbability(p1, p2, resultSet1, resultSet2, currentSet1, currentSet2, resultGem1, resultGem2, serve);
    cout << "Probability that player 1 wins the match: " << result * 100<<"%"<< endl; 

    return 0;
}
