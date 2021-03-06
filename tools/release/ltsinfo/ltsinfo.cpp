// Author(s): Muck van Weerdenburg
// Copyright: see the accompanying file COPYING or copy at
// https://github.com/mCRL2org/mCRL2/blob/master/COPYING
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)
//
/// \file ltsinfo.cpp

#define NAME "ltsinfo"
#define AUTHOR "Muck van Weerdenburg"

#include <string>

#include "mcrl2/utilities/logger.h"
#include "mcrl2/utilities/exception.h"
#include "mcrl2/utilities/input_tool.h"
#include "mcrl2/lts/lts_io.h"
#include "mcrl2/lts/lts_algorithm.h"

#include "mcrl2/lts/lts_lts.h"
#include "mcrl2/lts/lts_aut.h"
#include "mcrl2/lts/lts_fsm.h"
#include "mcrl2/lts/lts_dot.h"


using namespace mcrl2::utilities::tools;
using namespace mcrl2::utilities;
using namespace mcrl2::core;
using namespace mcrl2::log;

typedef input_tool ltsinfo_base;
class ltsinfo_tool : public ltsinfo_base
{

  private:

    std::string                 infilename;
    mcrl2::lts::lts_type        intype;
    bool                        print_state_labels;

  public:

    ltsinfo_tool() :
      ltsinfo_base(NAME,AUTHOR,
                   "display basic information about an LTS",
                   "Print information about the labelled transition system (LTS) in INFILE. "
                   "If INFILE is not supplied, stdin is used.\n"
                   "\n"
                   "The format of INFILE is determined by its contents. "
                   "The option --in can be used to force the format for INFILE. "
                   "The supported formats are:\n"
                   +mcrl2::lts::detail::supported_lts_formats_text()
                  ),
      intype(mcrl2::lts::lts_none),
      print_state_labels(false)
    {
    }

  protected:

    void add_options(interface_description& desc)
    {
      using namespace mcrl2::lts;

      ltsinfo_base::add_options(desc);

      desc.
      add_option("in", make_mandatory_argument("FORMAT"),
                 "use FORMAT as the input format", 'i').
      add_option("state-label",
                 "print the labels of states",'l') ;
    }

    void parse_options(const command_line_parser& parser)
    {
      using namespace mcrl2::lts;

      ltsinfo_base::parse_options(parser);

      if (0 < parser.arguments.size())
      {
        infilename = parser.arguments[0];
      }
      if (1 < parser.arguments.size())
      {
        throw parser.error("Too many file arguments.");
      }

      if (parser.options.count("in"))
      {
        if (1 < parser.options.count("in"))
        {
          throw parser.error("Multiple input formats specified; can only use one.");
        }

        intype = mcrl2::lts::detail::parse_format(parser.option_argument("in"));
        if (intype == lts_none || intype == lts_dot)
        {
          throw parser.error("Option -i/--in has illegal argument '" +
                       parser.option_argument("in") + "'.");
        }
      }

      print_state_labels=parser.options.count("state-label")>0;
    }

    template <class SL, class AL, class BASE>
    static void provide_probabilistic_information(const mcrl2::lts::lts<SL,AL,BASE>& )
    {
      // No probabilistic information is provided for a plain lts.
    }


    template <class SL, class AL, class PROBABILISTIC_STATE, class BASE>
    static void provide_probabilistic_information(mcrl2::lts::probabilistic_lts < SL, AL, PROBABILISTIC_STATE, BASE>&  l)
    {
      std::size_t count_non_trivial_probabilistic_states=0;
      if (l.initial_probabilistic_state().size()>1)
      {
        count_non_trivial_probabilistic_states++;
      }
      for(std::size_t i=0; i<l.num_probabilistic_states(); ++i)
      {
        if (l.probabilistic_state(i).size()>1)
        {
          count_non_trivial_probabilistic_states++;
        }
      }
      if (count_non_trivial_probabilistic_states>0)
      { 
        // The initial state can be probabilistic, so it is added separately. 
        mCRL2log(info) << "This lts has " << l.num_probabilistic_states()+1 << " probabilistic states.\n";
        mCRL2log(info) << "Out of these " << count_non_trivial_probabilistic_states << " contain" << ((count_non_trivial_probabilistic_states<2)?"s":"") 
                       << " a non trivial probability distribution.\n";
        mCRL2log(info) << "The initial state is " << ((l.initial_probabilistic_state().size()>1)?"":"not ") << "probabilistic.\n";
      }
      else
      {
        mCRL2log(info) << "This lts has no probabilistic states.\n";
      }

    }

    // Code to print the state labels. There is a specialisation for an probabilistic_lts_lts_t.
    void print_the_state_labels(const mcrl2::lts::probabilistic_lts_fsm_t& l) const
    {
      if (print_state_labels)
      {
        if (!l.has_state_info())
        {
          mCRL2log(info) << "This transition system has no state labels. Therefore they cannot be printed.\n";
        }
        else 
        {
          mCRL2log(info) << "The state labels of this .fsm format. Note that state labels in .fsm files are only partly preserved by state space reductions.\n";
          for(std::size_t i=0; i<l.num_states(); ++i)
          {
            mCRL2log(info) << i << ": " << pp(l.state_label(i)) << "\n";
          }
        }
      }
    }

    // Code to print the state labels. There is a specialisation for an probabilistic_lts_lts_t.
    void print_the_state_labels(const mcrl2::lts::probabilistic_lts_aut_t& ) const
    {
      if (print_state_labels)
      {
        mCRL2log(info) << "Transition systems in .aut format have no state labels. Therefore, they cannot be listed.\n"; 
      }
    }

    // Print the state labels for a probabilistic lts.
    void print_the_state_labels(const mcrl2::lts::probabilistic_lts_lts_t& l) const
    {
      if (print_state_labels)
      {
        if (!l.has_state_info())
        {
          mCRL2log(info) << "This transition system has no state labels. Therefore they cannot be printed.\n";
        }
        else 
        {
          mCRL2log(info) << "The state labels of this labelled transition system:\n";
          for(std::size_t i=0; i<l.num_states(); ++i)
          {
            if (l.state_label(i).size()==0)
            {
              mCRL2log(info) << i << ": no label.\n";
            }
            else 
            { 
              for(const mcrl2::lps::state& lab: l.state_label(i))
              { 
                mCRL2log(info) << i << ": (" << pp(lab) << ").\n";
              }
            }
          }
        }
      }
    }

    template < class LTS_TYPE >
    bool provide_information() const
    {
      LTS_TYPE l;
      l.load(infilename);

      mCRL2log(info) 
          << "Number of states: " << l.num_states() << ".\n"
          << "Number of action labels: " << l.num_action_labels() << " (including a tau label).\n"
          << "Number of transitions: " << l.num_transitions() << ".\n";

      if (l.has_state_info())
      {
        mCRL2log(info) << "Number of state labels: " << l.num_state_labels() << ".\n";
      }
      else
      {
        if (!print_state_labels) // This is to prevent the same message being printed twice.
        {
          mCRL2log(info) << "There are no state labels." << std::endl;
        }
      }

      mCRL2log(verbose) << "Checking reachability..." << std::endl;
      if (!reachability_check(l))
      {
        mCRL2log(info) << "Warning: some states are not reachable from the initial state! (This might result in unspecified behaviour of LTS tools.)" << std::endl;
      }

      mCRL2log(verbose) << "Checking whether lts is deterministic..." << std::endl;
      mCRL2log(info) << "LTS is ";
      if (!is_deterministic(l))
      {
        mCRL2log(info) << "not ";
      }
      mCRL2log(info) << "deterministic." << std::endl;

      provide_probabilistic_information(l);

      print_the_state_labels(l);

      return true;
    }



  public:

    bool run()
    {
      using namespace mcrl2::lts;
      using namespace mcrl2::lts::detail;

      if (intype==lts_none)
      {
        intype = guess_format(infilename);
      }

      switch (intype)
      {
        case lts_lts:
        {
          return provide_information<probabilistic_lts_lts_t>();
        }
        case lts_none:
          mCRL2log(warning) << "No input format is specified. Assuming .aut format.\n";
        case lts_aut:
        {
          return provide_information<probabilistic_lts_aut_t>();
        }
        case lts_fsm:
        {
          return provide_information<probabilistic_lts_fsm_t>();
        }
        case lts_dot:
        {
          throw mcrl2::runtime_error("Cannot read .dot files anymore.");
        }
      }
      return true;
    }
};

int main(int argc, char** argv)
{
  return ltsinfo_tool().execute(argc, argv);
}
