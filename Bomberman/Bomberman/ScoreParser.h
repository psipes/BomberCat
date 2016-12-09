#pragma once

#if !defined( _SCOREPARSER_H_ )
#define _SCOREPARSER_H_

#include <vector>

//========================================================================================
// ScoreParser class manages reading and writing to BCHighScore.txt
class ScoreParser
{

   //========================================================================================
   // structure presenting a highScore containing an int score and a tstring name.
   struct highScore
   {
      int score;
      tstring name;
   };

public:
   //========================================================================================
   // ctor
   ScoreParser();
   
   //========================================================================================
   // dtor
   ~ScoreParser();


   //===================================================================================================
   // Prepares this object for destruction.
   void destroy();

   //========================================================================================
   // reads the top five scores in BCHighScore.txt and adds them to a vector of ints 
   // to be accessed from somewhere else.
   void readScore();
   
   //========================================================================================
   // Writes and sorts players score to BCHighScore.txt and creates a BCHighScore.txt 
   // if none exists
   void writeScore( int score, tstring name );
   
   //========================================================================================
   // returns a int of the top five scores in BCHighScore.txt.
   highScore getScores( int scoreNum ){ return myScore[ scoreNum ]; }
   
private:

   tstring                myFilePath;
   std::vector<highScore> myScore;

};
#endif //_SCOREPARSER_H_