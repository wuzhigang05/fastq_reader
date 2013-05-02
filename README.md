
fastq_reader --id         Only display IDs
fastq_reader -f           Only display fasta
fastq_reader --seqonly    Only display seqs


Example usage: 
	Fastq_reader --id|--seqonly|--fasta <fastq files> OR
	cat <fastq files> | Fastq_reader --id|--seqonly|--fasta:
  -h [ --help ]               produce help message
  -I [ --id ]                 only prints the seqids to the STDOUT, which 
                              isincompatible with fasta and input option 
                              [boolean]
  --seqonly                   only print the sequence to the STDOUT [boolean]
  -f [ --fasta ]              print the fasta to the STDOUT [boolean]
  -o [ --output ]             output file. default [STDOUT] [str]
  -i [ --input ] arg (=STDIN) input fastq files [str]
  -n [ --number ] arg         retrieve the specified number of records. [int]

