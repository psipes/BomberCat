#include "stdafx.h"
#include "Utilities/TTypes.h"
#include "Utilities/Logger.h"
#include "Utilities/PathUtilities.h"
#include "Utilities/ConfigParser.h"
#include "DxFramework/DxFramework.h"
#include "DxFramework/DxCommon.h"
#include "DxFramework/DxAssetManager.h"
#include "Bomberman/ScoreParser.h"


ScoreParser::ScoreParser()
{
   
}

//===================================================================================================>
//
ScoreParser::~ScoreParser()
{
   
}

//===================================================================================================>
//
void ScoreParser::destroy()
{
   myFilePath.clear();
   myScore.clear();
   myScore.resize(0);
}



//===================================================================================================>
//reads the top five scores in BCHighScore.txt and adds them to a vector of ints to be accessed
//from somewhere else
void ScoreParser::readScore()
{
   myScore.resize( 5 );
   tstring configFilename = "BCHighScore.txt";
   myFilePath = DxAssetManager::getInstance().getConfigAssetPath( configFilename );
   tifstream      configFile( myFilePath.c_str() );

   if ( !configFile.is_open() || configFile.bad() )
   {
      //if BCHighScore.txt is not found then this makes a new one
      tofstream configFileOut( myFilePath.c_str() );
      configFileOut <<  "AAA" << " " << "0" << endl;
      configFileOut.close();
   }
  
   ConfigParser   parser;
   tstring        line;
   //parser.getNextLine( configFile, line );

   for( unsigned int i = 0; i < myScore.size(); i++)
   {
      parser.getNextLine( configFile, line );
      tstringstream ss( line );
      tstring name;
      int score = 0;
      ss >> name >> score;

      myScore[i].name = name;
      myScore[i].score = score;
   }
   configFile.close();
}

//===================================================================================================>
//Writes and sorts players score to BCHighScore.txt and creates a BCHighScore.txt if non exists
void ScoreParser::writeScore( int score, tstring name )
{
   highScore newScore;
   newScore.score = score;
   newScore.name = name;
   tstring configFilename = "BCHighScore.txt";
   myFilePath = DxAssetManager::getInstance().getConfigAssetPath( configFilename );
   tifstream configFileIn( myFilePath.c_str(), std::ios::app );

   if ( !configFileIn.is_open() || configFileIn.bad() )
   {
      //if BCHighScore.txt is not found then this makes a new one
      tofstream configFileOut( myFilePath.c_str() );
      configFileOut <<  "AAA" << " " << "0" << endl;
      configFileOut.close();
   }
      
   ConfigParser   parser;
   tstring        line;

   std::vector<highScore> scores;
   int index = 0;
   if ( !parser.getNextLine( configFileIn, line ) )
   {
      //if no scores are found in BCHighScore.txt inserts a 0 as a place holder
      tofstream configFileOut( myFilePath.c_str() );
      configFileOut << "AAA" << " " <<  "0" << endl;
      configFileOut.close();
   }

   do
   {
      tstringstream ss( line );
      highScore score;
      score.score = 0;
      ss >> score.name >> score.score;
      ss.clear();

      if ( newScore.score >= score.score )
      {          
         scores.push_back( newScore );
         newScore.score = -1;
      }

      scores.push_back( score );
   }while ( parser.getNextLine( configFileIn, line ) );

   configFileIn.close();
   tofstream configFileOut( myFilePath.c_str() );
   if ( !configFileOut.is_open() || configFileOut.bad() )
      return;

   for( unsigned int i = 0; i < scores.size(); i++)
   {
      configFileOut << scores[i].name << " " << scores[i].score << std::endl;
   }

   configFileOut.close();
}