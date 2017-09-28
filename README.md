# Pallindromic-Sequence-in-Nucleic-Acid (Linear Time Algorithm)


1. inv.c - This file generates the string complement pair of a given input string.

Command - a.) make inv
b.) ./inv -input <input_file_name> -output <output_file_name>

2. seqFinder.c - This file points out all the indices of the pattern, along with the indices of the gap, present in a string.

Command -
a.) make seqFinder
b.) ./SeqFinder -srt <srt_file_name_with_path> -lcp <lcp_file_name_with_path> -output <output_file_name_with_path> 


Output Example:

Initial           Final          Count         InitialGap    FinalGap      String

12308 	          12315 	        8 		        0 			      0	 	          AAAATTTT

2099 	            2124 	          20 		        2109 		      2114 	        GTCCAAAGAGGAACAGCTCTTTGGAC

Time Complexity:

For constant "allowed gap value" and "number of total such sequences" the time complexity is O(n). 

Explanation:
It is O(stringLength * AllowedGap * size). However, the gap value is constant (default value is 10) and also the "number of total such sequences" is very less compared to the size of the string "n". That is why, we can ignore those values as they are negligible compared to the size of the string and thus, the time complexity becomes linear.



Acknowledgment:
I would like to thank Dr.M.Ganapathiraju for her guidance in completing the project.

References:
1. https://bmcbioinformatics.biomedcentral.com/articles/10.1186/1471-2105-12-12
2. https://www.ncbi.nlm.nih.gov/pmc/articles/PMC3602881/
3. https://www.ncbi.nlm.nih.gov/pubmed/15693744
