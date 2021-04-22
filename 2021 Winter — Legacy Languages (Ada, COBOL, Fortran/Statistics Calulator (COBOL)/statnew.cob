*> statnew.cob
*>
*> Prompts user for input file of numbers, performs calculations and
*> writes results to output file of user's choosing
*> 
*> Written by James Singaram
*> 0758172
*> jsingara@uoguelph.ca
*> March 22, 2021
*> 

identification division.
program-id. Stats.

environment division.
input-output section.
file-control.
select input-file assign to input-file-name
    organization is line sequential.
select output-file assign to output-file-name
    organization is line sequential.


data division.
file section.
fd input-file.
    01 input-record.
        05 input-num    pic S9(14)V9(4).
fd output-file.
    01 output-string    pic X(40).


working-storage section.
01 eof-switch       pic 9 value 1.

01 input-file-name  pic X(100).
01 output-file-name pic X(100).

01 new-numbers.
    05 num          pic S9(14)V9(4) usage is computational-3 
        occurs 1000 times.
01 sorted-numbers.
    05 num-sorted   pic S9(14)V9(4) usage is computational-3 
        occurs 1000 times.

01 text-string      pic X(40).
01 line-output. 
    05 filler       pic X(15).
    05 num-string       pic -(14)9.9(4).

01 num-sum          pic S9(14)V9(4).
01 num-mean         pic S9(14)V9(4).
01 num-stddev       pic S9(14)V9(4).
01 num-var          pic S9(14)V9(4).
01 squared-temp     pic S9(14)V9(4).
01 log-num          pic S9(14)V9(14).
01 temp-geo-mean    pic S9(14)V9(14).
01 num-geo-mean     pic S9(14)V9(4).
01 harm-sum         pic S9(14)V9(14).
01 num-harm-mean    pic S9(14)V9(4).
01 num-temp         pic S9(14)V9(4).
01 num-median       pic S9(14)V9(4).

01 i                pic 9999.
01 j                pic 9999.
01 k                pic 9999.
01 num-records      pic 9999.


procedure division.

    perform start-program.
    perform get-inputs.

    perform open-files.
    perform read-numbers.

    perform calc-mean.
    perform calc-stddev.
    perform calc-geometric-mean.
    perform calc-harmonic-mean.
    perform copy-arr-to-sort.
    perform bubble-sort.
    perform calc-median.

    perform write-results.

    perform close-files.
    perform close-program.

stop run.
    

*> Displays welcome message to user
start-program.
    display " ".
    display "Statistics calculator".
    display " ".

*> Prompts and collects input and output file names from user
get-inputs.
    display " Input file name: ".
    accept input-file-name.
    display " Output file name: ".
    accept output-file-name.

*> Opens input and output files
open-files.
    open input input-file.
    open output output-file.

*> Closes input and output files
close-files.
    close input-file.
    close output-file.

*> Reads all numbers in input file 
read-numbers.
    move 0 to i.
    perform read-file
        until eof-switch = 0.
    compute num-records = i.

*> Reads records from the input file 
read-file.
    read input-file into input-record
        at end move zero to eof-switch
    end-read.
    if eof-switch is not equal to zero
        move input-num to num(i)
        add 1 to i
    end-if.

*> Calculates the mean of the numbers
calc-mean.
    perform calc-sum.
    compute num-mean = num-sum / num-records.

*> Calculates the sum of numbers
calc-sum.
    move 0 to i.
    move 0.0 to num-sum.
    perform add-num-to-sum
        until i = num-records.

*> Adds one number to sum
add-num-to-sum.
    compute num-sum = num-sum + num(i).
    add 1 to i.

*> Calculates the standard deviation
calc-stddev.
    move 0 to i.
    move 0.0 to num-stddev.
    perform get-squared-deviations
        until i = num-records.
    compute num-stddev = num-stddev / num-records.
    move num-stddev to num-var.
    compute num-stddev = num-stddev ** 0.5.

*> Calculates and adds sum of squared deviation to stddev
get-squared-deviations.
    compute squared-temp = ((num(i) - num-mean) ** 2).
    add squared-temp to num-stddev.
    add 1 to i.

*> Calculates the geometric mean of the numbers
calc-geometric-mean.
    move 0 to i.
    move 0.0 to temp-geo-mean.
    perform calc-geo-product
        until i = num-records.
    compute temp-geo-mean = temp-geo-mean / num-records.
    compute num-geo-mean = 10 ** temp-geo-mean.

*> Calculates the sum of numbers
calc-geo-product.
    compute log-num = function log10 (num(i)).
    add log-num to temp-geo-mean.
    add 1 to i.

*> Calculates the harmonic mean of the numbers
calc-harmonic-mean.
    move 0 to i.
    move 0.0 to harm-sum.
    move num-records to temp-geo-mean.
    perform calc-harm-sum
        until i = num-records.
    compute num-harm-mean = num-records / harm-sum.

*> Calculates sum of inverse numbers
calc-harm-sum.
    compute harm-sum = harm-sum + (1 / num(i)).
    add 1 to i.

*> Creates copy of num array
copy-arr-to-sort.
    move 0 to i.
    perform create-copy
        until i = num-records.

*> Copies one element of num array to num-sorted array
create-copy.
    move num(i) to num-sorted(i).
    add 1 to i.

*> Runs bubble sort on array of nums
bubble-sort.
    move 0 to i.
    perform bubble-sort-2
        until i = num-records.

*> One sorting pass through array
bubble-sort-2.
    move 1 to j.
    perform sort-compare
        until j = num-records.
    add 1 to i.

*> Compares two elements of array and swaps if necessary
sort-compare.
    compute k = j - 1.
    if (num-sorted(k) > num-sorted(j)) then
        move num-sorted(j) to num-temp
        move num-sorted(k) to num-sorted(j)
        move num-temp to num-sorted(k)
    end-if.
    add 1 to j.

*> Calculates the median value in the array
calc-median.
    compute num-temp = function mod (num-records, 2).
    if (num-temp = 1) then
        compute i = num-records / 2
        move num-sorted(i) to num-median
    else
        compute i = num-records / 2
        compute j = i - 1
        compute num-median = (num-sorted(i) + num-sorted(j)) / 2
    end-if.

*> Writes results to output file
write-results.
    perform write-header.
    perform write-stats.
    perform write-unsorted.
    perform write-sorted.

*> Writes header text to output file
write-header.
    move '===================================' to text-string.
    write output-string from text-string 
        after advancing 0 lines.
    move '||                               ||' to text-string.
    write output-string from text-string 
        after advancing 1 line.
    move '||     Statistics Calculator     ||' to text-string.
    write output-string from text-string 
        after advancing 1 line.
    move '||                               ||' to text-string.
    write output-string from text-string 
        after advancing 1 line.
    move '===================================' to text-string.
    write output-string from text-string 
        after advancing 1 line.
    
*> Writes statistics to output file
write-stats.
    move '        Statistical Results        ' to text-string.
    write output-string from text-string 
        after advancing 2 lines.
    move '-----------------------------------' to text-string.
    write output-string from text-string 
        after advancing 1 line.
    perform write-mean.
    perform write-geo-mean.
    perform write-harm-mean.
    perform write-median.
    perform write-var.
    perform write-stddev.

*> Writes mean to output file
write-mean.    
    move ' Mean ' to line-output.
    move num-median to num-string.
    write output-string from line-output 
        after advancing 1 line.

*> Writes geometric mean to output file
write-geo-mean.    
    move ' Geometric Mean' to line-output.
    move num-median to num-string.
    write output-string from line-output 
        after advancing 1 line.


*> Writes harmonic mean to output file
write-harm-mean.    
    move ' Harmonic Mean ' to line-output.
    move num-harm-mean to num-string.
    write output-string from line-output 
        after advancing 1 line.


*> Writes median to output file
write-median.    
    move ' Median ' to line-output.
    move num-median to num-string.
    write output-string from line-output 
        after advancing 1 line.

*> Writes variance to output file
write-var.    
    move ' Variance ' to line-output.
    move num-var to num-string.
    write output-string from line-output 
        after advancing 1 line.

*> Writes standard deviation to output file
write-stddev.    
    move ' Std Deviation ' to line-output.
    move num-stddev to num-string.
    write output-string from line-output 
        after advancing 1 line.

*> Writes unsorted nums from input file to output file
write-unsorted.
    move '      Unsorted Numbers in File      ' to text-string.
    write output-string from text-string 
        after advancing 2 lines.
    move '-----------------------------------' to text-string.
    write output-string from text-string 
        after advancing 1 line.
    move 0 to i.
    perform write-unsorted-num
        until i = num-records.

*> Writes the next unsorted num from input file to output file
write-unsorted-num.
    move '                ' to line-output.
    move num(i) to num-string.
    write output-string from line-output 
        after advancing 1 line.
    add 1 to i.

*> Writes sorted num array to output file
write-sorted.
    move '       Sorted Numbers in File       ' to text-string.
    write output-string from text-string 
        after advancing 2 lines.
    move '-----------------------------------' to text-string.
    write output-string from text-string 
        after advancing 1 line.
    move 0 to i.
    perform write-sorted-num
        until i = num-records.

*> Writes next sorted num array to output file
write-sorted-num.
    move '                ' to line-output.
    move num-sorted(i) to num-string.
    write output-string from line-output 
        after advancing 1 line.
    add 1 to i.

*> Closing message and terminates program
close-program.
    display "Thank you for using our statistics calculator.".
    display "Results have been saved in file " output-file-name.
    stop run.
