
1. inv.c - This file generates the string complement pair of a given input string.

Command - a.) make inv
b.) ./inv -input <input_file_name> -output <output_file_name>

2. seqFinder.c - This file points out all the indices of the pattern, along with the indices of the gap, present in a string.

Command -
a.) make seqFinder
b.) ./SeqFinder -srt <srt_file_name_with_path> -lcp <lcp_file_name_with_path> -output <output_file_name_with_path> 

Output Format Example:

Initial           Final          Count         InitialGap    FinalGap      String

12308 			  12315 		   8 				0 			0	 	  AAAATTTT

2099 			  2124 			  20 				2109 		2114 	GTCCAAAGAGGAACAGCTCTTTGGAC

Explanation of output format of seqFinder.c 

For example:
If the string is "XYZABCDGHABCDX". Then the output according to the index will be:

Initial	    Final		Count      InitialGap       FinalGap
3			12		   4              7                    8

Here the Gap is 2 (i.e, GH) and its starting index is 7 and ending index is 8 in the given string.


Acknowledgment:
I would like to thank Dr.M.Ganapathiraju for her guidance in completing the project.

References:
1. https://bmcbioinformatics.biomedcentral.com/articles/10.1186/1471-2105-12-12
2. https://www.ncbi.nlm.nih.gov/pmc/articles/PMC3602881/
