



	/**
         * @brief JUDAH - Jacob is equipped with a text-based user interface
         *
         * @file samu.cpp
         * @author  Norbert Bátfai <nbatfai@gmail.com>
         * @version 0.0.1
         *
         * @section LICENSE
         *
         * Copyright (C) 2015 Norbert Bátfai, batfai.norbert@inf.unideb.hu
         *
         * This program is free software: you can redistribute it and/or modify
         * it under the terms of the GNU General Public License as published by
         * the Free Software Foundation, either version 3 of the License, or
         * (at your option) any later version.
         *
         * This program is distributed in the hope that it will be useful,
         * but WITHOUT ANY WARRANTY; without even the implied warranty of
         * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
         * GNU General Public License for more details.
         *
         * You should have received a copy of the GNU General Public License
         * along with this program.  If not, see <http://www.gnu.org/licenses/>.
         *
         * @section DESCRIPTION
         *
         * JACOB, https://github.com/nbatfai/jacob
         *
         * "The son of Isaac is Jacob." The project called Jacob is an experiment
         * to replace Isaac's (GUI based) visual imagination with a character console.
         *
         * ISAAC, https://github.com/nbatfai/isaac
         *
         * "The son of Samu is Isaac." The project called Isaac is a case study
         * of using deep Q learning with neural networks for predicting the next
         * sentence of a conversation.
         *
         * SAMU, https://github.com/nbatfai/samu
         *
         * The main purpose of this project is to allow the evaluation and
         * verification of the results of the paper entitled "A disembodied
         * developmental robotic agent called Samu Bátfai". It is our hope
         * that Samu will be the ancestor of developmental robotics chatter
         * bots that will be able to chat in natural language like humans do.
         *
         */

#include <iostream>
#include <string>
#include <sstream>
#include "samu.hpp"

#include <sys/time.h>
#include <sys/types.h>
#include <unistd.h>

#include <ncurses.h>

#include <regex>
#include <string>
#include <curl/curl.h>
#include <vector>
#include <string>

#include <stdio.h>
#include <time.h>

std::string Samu::name {"Amminadab"};

#ifdef DISP_CURSES
Disp Samu::disp;

static size_t
WriteCallback (void *contents, size_t size, size_t nmemb, void *userp)
{
  ((std::string *) userp)->append ((char *) contents, size * nmemb);
  return size * nmemb;
}

void Samu::splitText (std::string text)
{
  size_t pos = 0;
  for (int i = 0; i < text.length (); i++)
    {
      if (text[i] == '\n' || text[i] == '.' || text[i] == '?'
	  || text[i] == '!')
	{
	  if (i == 0 || text[0] == ' ' || text[0] == '\t' || text[0] == '\n')
	    {
	      i=0;
	      text.erase (0, 1);
	      continue;
	    }
	  else
	    {
	      text.erase (i, 1);
	      internet_sentences.push_back (text.substr (0, i));
	      internet_sentences[internet_sentences.size () - 1].append ("\n");
	      text.erase (0, i);
	      i = 0;
	      while (text[i] == '\n' || text[i] == '.' || text[i] == '?'
		     || text[i] == '!' || text[i] == ' ' || text[0] == '\t')
		{
		  text.erase (0, 1);
		}
	    }
	}
    }
}

void Samu::removeTags (std::string text)
{
  std::cerr << "Done parsing" << std::endl;
  
  std::vector < std::string > internet_temp;

  size_t pos = 0;
  size_t pos2 = 0;
  size_t pos3 = 0;
  
  size_t tmppos = 0;
  size_t posScript1 = 0;
  size_t posScript2 = 0;
  
  std::string newurl;
  std::vector < std::string > links;
  links.clear ();
  
  while((tmppos = text.find("<script>")) != std::string::npos)
  {
    posScript1 = tmppos;
    posScript2 = text.find ("</script>");
    text.erase(posScript1, (posScript2-posScript1)+9);
  } 
  
  std::cerr << "Scripts removed" << std::endl;
  
  while ((pos = text.find ("&amp;")) != std::string::npos)
    {
      text.erase (pos + 1, 4);
    }
    
  std::cerr << "--------------- Links: ---------------" << std::endl;

  while ((pos = text.find ('<')) != std::string::npos)
    {
      if ((pos3 = text.find ("<a href=\"http")) != std::string::npos)
	{
	  pos3 += 9;
	  for (unsigned i = pos3; i < text.length (); i++)
	    {
	      if (text[i] == '\"')
		{
		  newurl = text.substr (pos3, i - pos3);
		  text.erase (pos3, i + 1 - pos3);
		  links.push_back (newurl);
		  std::cerr << newurl << std::endl;
		  break;
		}
	    }
	}

      if (pos != 0)
	{
	  internet_temp.push_back (text.substr (0, pos));
	  if (internet_temp[internet_temp.size () - 1][pos - 1] != '.'
	      && internet_temp[internet_temp.size () - 1][pos - 1] != '!'
	      && internet_temp[internet_temp.size () - 1][pos - 1] != '?'
	      && internet_temp[internet_temp.size () - 1][pos - 1] != '\n')
	    {
	      internet_temp[internet_temp.size () - 1].append ("\n");
	    }
	  text.erase (0, pos);
	}
      else if ((pos2 = text.find ('>')) != std::string::npos)
	{
	  text.erase (pos, pos2 + 1 - pos);
	}
    }
  if (text.length () != 0)
    {
      internet_temp.push_back (text);
    }

for (auto str:internet_temp)
    {
      splitText (str);
    }
    
  std::cerr << "--------------- Sentences: ---------------" << std::endl;
    
    
  while (internet_row < internet_sentences.size ())
    {
      if (internet_sentences[internet_row] != "\n" && internet_sentences[internet_row].length() > 10)
	{
	  if(internet_sentences[internet_row].length() > 0 )
	  {    
	  sentence (-1, internet_sentences[internet_row]);
	  std::cerr << internet_sentences[internet_row] << std::endl; 
	  }
	}
	internet_row++; 
    }
    
  internet_sentences.clear ();
  internet_row = 0;
  
  if (links.size () > 0)
    {
      internet_sentences.clear();
      int random = rand () % (links.size ());
      disp.log (links[random] + " (" + std::to_string(random) + "/" + std::to_string(links.size()) + ")");
      std::cerr << links[random] << " (" + std::to_string(random) << "/" << std::to_string(links.size()) << ")" << std::endl;
      parseURL (links[random]);
    }
    else
    {
      disp.log("No links found");
      is_surfing = false;
    }
    
}

void Samu::parseURL (std::string url)
{
  disp.log ("Parsing");
  CURL *curl;
  CURLcode res;
  std::string readBuffer;

  curl = curl_easy_init ();
  if (curl)
    {
      curl_easy_setopt (curl, CURLOPT_URL, url.c_str ());
      curl_easy_setopt (curl, CURLOPT_WRITEFUNCTION, WriteCallback);
      curl_easy_setopt (curl, CURLOPT_WRITEDATA, &readBuffer);
      res = curl_easy_perform (curl);

      if (res != CURLE_OK)
	{
	  readBuffer = "";
	}

      curl_easy_cleanup (curl);
    }

  removeTags (readBuffer);
}

void Samu::FamilyCaregiverShell (void)
{
  std::string cmd_prefix = "cmd";

  fd_set rfds;
  struct timeval tmo;

  int sleep{0};

  if (sleep_) sleep = sleep_after_ + 1;

  int prev_sec{0};
  
  for (; run_;)
    {

      if (is_surfing && internet_sentences.size () != 0)
	{
	  if (internet_row < internet_sentences.size ())
	    {
	      sentence (-1, internet_sentences[internet_row]);
	      internet_row++;
	    }
	  else
	    {
	      //is_surfing = false;
	      internet_sentences.clear ();
	      internet_row = 0;
	    }
	}
      else
	{
	  //is_surfing = false;
	  internet_sentences.clear ();
	  internet_row = 0;
	}

      try
      {

	disp.cg_read ();

	if (!is_surfing)
	  {
	    if (++sleep > sleep_after_)
	      {
		if (!sleep_)
		  {
		    std::cerr << "Isaac went to sleep." << std::endl;
		    disp.log ("I went to sleep.");
		  }
		sleep_ = true;
	      }
	    else
	      {
		std::cerr << sleep << " " << std::flush;

		int sec = (sleep * read_usec_) / (1000 * 1000);
		if (sec != prev_sec)
		  {
		    int after = (sleep_after_ * read_usec_) / (1000 * 1000);

		    std::stringstream sleep_after;

		    sleep_after << "I will go to sleep after ";
		    sleep_after << (after - sec);
		    sleep_after << " seconds";

		    disp.log (sleep_after.str ());
		    prev_sec = sec;
		  }
	      }
	  }
      }
      catch (std::string line)
      {

	if (is_surfing)
	  {
	    is_surfing = false;
	    internet_sentences.clear ();
	    internet_row = 0;
	  }

	if (sleep_)
	  {
	    std::cerr << "Isaac is awake now." << std::endl;
	    disp.log ("I am awake now");
	  }
	sleep_ = false;
	sleep = 0;

	if (!line.compare (0, cmd_prefix.length (), cmd_prefix))
	  {
	    std::string readCmd
	    {
	    "cmd read"};

	    size_t f = line.find (readCmd);
	    if (f != std::string::npos)
	      {
		f = f + readCmd.length () + 1;
		if (f < line.length ())
		  {
		    std::string fname = line.substr (f);
		    set_training_file (fname);
		  }
	      }
	    else
	      NextCaregiver ();
	  }
	else if (std::regex_match (line, std::regex ("http.*")))
	  {
	    disp.log ("I am surfing on the internet.");
	    is_surfing = true;
	    internet_row = 0;
	    parseURL (line);
	  }
	else
	  {
	    try
	    {
	      sentence (-1, line);
	    }
	    catch (const char *err)
	    {
	      std::cerr << err << std::endl;
	      disp.log (err);
	    }
	  }
      }

      usleep (read_usec_);

    }

  run_ = false;
}

#else

void
Samu::FamilyCaregiverShell (void)
{
  for (; run_;)
    usleep (read_usec_);

  run_ = false;
}

#endif
