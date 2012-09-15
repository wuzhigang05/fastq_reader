/*
 * =====================================================================================
 *
 *       Filename:  fastq_reader.cc
 *
 *    Description:  This is a file that can read in fastq either from STDIN or
 *                  a file.
 *
 *        Version:  1.0
 *        Created:  08/09/2012 15:42:59
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  Zhigang Wu (UCR), zhigang.wu@email.ucr.edu
 *   Organization:  
 *
 * =====================================================================================
 */

/* Copyright 2012 by Zhigang Wu
 * Department of Botany and Plant Sciences
 * University of California
 * Riverside, CA 92527, USA
 */

#include <iostream>
#define PROG "fastq_reader"
#include <boost/program_options.hpp>
#include <vector>
#include <string>
#include "readRecord.h"
using namespace std;
namespace po = boost::program_options;
static std::string format;


template <typename TStream>
string  get_Fastaq_format(TStream & stream)
{
  std::string format;
  if (stream.peek() == '>')
    format = "fasta";
  else if (stream.peek() == '@')
    format = "fastq";
  else
    errx(1, "unable to determine format");
  return format;
}

std::string get_format_app(const po::variables_map & vm)
{
  std::string format;
  if (vm["input"].as<vector<string> >().size() == 1 &&
      vm["input"].as<vector<string> >()[0] == string("stdin") )
  {
    if (!isatty(fileno(stdin)))
      format = get_Fastaq_format(std::cin);
  }
  else
  {
    string filename;
    filename = vm["input"].as<vector<string> >()[0]; 
    ifstream stream(filename.c_str());
    if (!stream.good())
      errx(1, "cannot open file %s for read", filename.c_str());
    format = get_Fastaq_format(stream);
    stream.close();
  }
  return format;
}

template<typename TStream>
void process(TStream & stream, bool & SeqOnly, bool & IDOnly, bool & FastaOnly)
{
  Seqrecord seq;
  if (SeqOnly)
    while(ReadRecord(stream, seq, format.c_str()))
      std::cout << seq.Seq << std::endl;
  else if (IDOnly)
    while(ReadRecord(stream, seq, format.c_str()))
      std::cout << seq.ID << std::endl;
  else if (FastaOnly)
    while(ReadRecord(stream, seq, format.c_str()))
      WriteRecord(std::cout, seq, FASTA());
  else
    while(ReadRecord(stream, seq, format.c_str()))
      WriteRecord(std::cout, seq, format.c_str());
}

int main (int argc, char * argv []) 
{
  try
  {
    po::options_description desc(
        "Part of NGS C++ Toolkit by Zhigang Wu zhigang.wu@email.ucr.edu\n"
        "Example usage: \n\tFastq_reader --id|--seqonly|--fasta <fastq files> OR"
        "\n\tcat <fastq files> | Fastq_reader --id|--seqonly|--fasta"
        );
    desc.add_options()
      ("help,h", "produce help message")
      ("id,I", "only prints the seqids to the STDOUT, which is"
       "incompatible with fasta and input option [boolean]")
      ("seqonly", "only print the sequence to the STDOUT [boolean]")
      ("fasta,f", "print the fasta to the STDOUT [boolean]")
      ("output,o", "output file. default [STDOUT] [str]")
      ("input,i", po::value<vector<string> >()->default_value(vector<string >(1,"stdin"), 
                                                              "STDIN"), 
       "input fastq files [str]")
      ("number,n", po::value<int>(), "retrieve the specified number of records. [int]")
//      ("format", po::value<string>()->default_value("fastq", "fastq"), 
//       "though the program is called fastq_reader, if you specify fasta format here," 
//       "all the options specified here will also work. [str]")
      ;


    po::positional_options_description p;
    p.add("input", -1);

    po::variables_map vm;
    po::store(po::command_line_parser(argc, argv).
              options(desc).positional(p).run(), vm);
    po::notify(vm);
    
    if (vm.count("help")) 
    {
      cerr << desc << "\n";
      return 1;
    }
// at least we need one input file
    if (!vm.count("input"))
    {
      cerr << "You have to specify at least on fastq file" << endl;        
    }
// only one of three mode can be specified
    if (vm.count("id") && vm.count("fasta") && vm.count("seqonly"))
    {
      cerr << "Only one of id, seqonly, fasta mode can be specified" << endl;
      exit(1);
    }
    else if (vm.count("id") && vm.count("fasta"))
    {
      cerr << "Only one of id, seqonly, fasta mode can be specified" << endl;
      exit(1);
    }
    else if (vm.count("fasta") && vm.count("seqonly"))
    {
      cerr << "Only one of id, seqonly, fasta mode can be specified" << endl;
      exit(1);
    }
    else if (vm.count("id") && vm.count("seqonly"))
    {
      cerr << "Only one of id, seqonly, fasta mode can be specified" << endl;
      exit(1);
    }

  format = get_format_app(vm);
//    bool FastaOnly = vm["format"].as<string>() == string("fasta") ? true : false;
    bool FastaOnly = vm.count("fasta");
    bool IDOnly =  vm.count("id");
    bool SeqOnly = vm.count("seqonly");
    if (vm["input"].as< vector<string> >().size() == 1 and !isatty(fileno(stdin)) )
      process(std::cin, SeqOnly, IDOnly, FastaOnly);
    else if (vm["input"].as<vector<string> >().size() > 0)
    {   
      string filename;
      for (int i = 0; i < vm["input"].as<vector<string> >().size(); ++i)
      {
        filename = vm["input"].as<vector<string> >()[i]; 
        ifstream stream(filename.c_str());
        if (!stream.good())
          errx(1, "cannot open file %s for read", filename.c_str());
        process(stream, SeqOnly, IDOnly, FastaOnly);
      }
    }
    else
    {
      cerr << "Error in line:" << __LINE__ << " " << __FILE__ << endl;
      cerr << "You have to specify at least one input file" << endl;
      cerr << desc << endl;
      exit(1);
    }

  }catch(exception & e)
  {
    cerr << e.what() << endl;
    return 1;
  }
}


