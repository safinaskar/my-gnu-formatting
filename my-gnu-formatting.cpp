// Написать нормально будет сложно, поэтому пишем совсем запростецкую вещь

#include <stdlib.h>
#include <assert.h>

#include <iostream>
#include <string>
#include <regex>

using namespace std;

void
my_err (const string &message)
{
  cerr << "Error: " << message << "\n";
  exit (EXIT_FAILURE);
}

bool
get (string &line)
{
  for (;;)
    {
      if (!getline (cin, line))
        {
          return false;
        }

      for (;;)
        {
          if (line.empty ())
            {
              break;
            }

          if (line.front () != ' ' && line.front () != '\t')
            {
              return true;
            }

          line.erase (0, 1);
        }

      cout << "\n";
    }
}

string
force_get (void)
{
  string line;

  if (!get (line))
    {
      my_err ("unexpected EOF");
    }

  return line;
}

int indent = 0;

void
put (string s)
{
  for (;;)
    {
      assert (!s.empty ());

      if (s.back () != ' ' && s.back () != '\t')
        {
          break;
        }

      s.pop_back ();
    }

  for (int i = 0; i != indent; ++i)
    {
      cout << "  ";
    }

  cout << s << "\n";
}

void
compound (void);

void
op (string line)
{
  if (regex_match (line, regex ("(if|else|while|do|for|switch)\\b.*")))
    {
      bool is_do = regex_match (line, regex ("do\\b.*"));

      if (line.back () == '{')
        {
          line.pop_back ();
          put (line);
          ++indent;
          put ("{");
          compound ();
          --indent;
        }
      else if (line.back () == ';')
        {
          line.pop_back ();
          put (line);
          ++indent;
          put (";");
          --indent;
        }
      else
        {
          put (line);
          ++indent;
          line = force_get ();

          if (line == "{")
            {
              put ("{");
              compound ();
            }
          else if (line == "}")
            {
              my_err ("unexpected }");
            }
          else if (line == ";")
            {
              put (";");
            }
          else
            {
              put ("{");
              ++indent;
              op (line);
              --indent;
              put ("}");
            }

          --indent;
        }

      if (is_do)
        {
          line = force_get ();
          assert (regex_match (line, regex ("^while")));
          put (line);
        }
    }
  else if (line.back() == '{')
    {
      // For example, class in class
      line.pop_back ();
      put (line);
      put ("{");
      compound ();
    }
  else
    {
      put (line);
    }
}

void
compound (void)
{
  ++indent;

  for (;;)
    {
      string line = force_get ();

      if (line == "{")
        {
          // Rare case
          put ("{");
          compound ();
        }
      else if (line == "}" || line == "};")
        {
          --indent;
          put (line);
          break;
        }
      else if (line.front () == '}')
        {
          // "}else{" or "}else if(...){"
          --indent;
          put ("}");
          --indent;
          line.erase (0, 1);
          op (line);
          ++indent;
          break;
        }
      else
        {
          op (line);
        }
    }
}

int
main (void)
{
  string line;

  while (get (line))
    {
      if (line.back () == '{')
        {
          line.pop_back();

          if (!line.empty ())
            {
              put (line);
            }

          put ("{");
          compound ();
          assert (indent == 0);
        }
      else
        {
          put (line);
        }
    }

  exit (EXIT_SUCCESS);
}
