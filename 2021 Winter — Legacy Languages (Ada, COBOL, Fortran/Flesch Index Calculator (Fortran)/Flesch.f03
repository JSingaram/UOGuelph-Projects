program flesch_calculator

    implicit none
    character (len=200) :: filename, outputFilename
    integer :: numSyll, numWord, numSent, fileSize
    real:: fleschIndex, gradeLevel
    logical fexist



    call start_program
    call get_input_filename
    call validate_file

    if (fexist) then
        call open_file
        call get_file_size
        call analyze_file (numSyll, numWord, numSent)
        call close_file
        call calculate_output 
        call get_output_filename
        call open_output_file
        call output_results
        call close_file
    else
        call no_file()
    end if

    call end_program()



    contains

    ! Displays opening message for program
    subroutine start_program
        write (*,*) ' '
        write (*,*) 'Flesch Index Calculator'
        write (*,*) ' '
    end subroutine start_program


    ! Prompts user for filename to be assessed
    subroutine get_input_filename
        write (*,*) 'Please enter the name of the file to analyze: '
        read (*,*) filename
    end subroutine get_input_filename


    ! Prompts user for filename to store results in
    subroutine get_output_filename
        write (*,*) 'Please enter the name of the output file: '
        read (*,*) outputFilename
    end subroutine get_output_filename


    ! Ensures that user's input file exists
    subroutine validate_file
        inquire(file = filename, exist = fexist)
    end subroutine validate_file


    ! Opens file that was input by user
    subroutine open_file
        open(unit = 69, file = filename, form = 'formatted', status = 'old', action = 'read')
    end subroutine open_file


    ! Opens the output file specified by the user
    subroutine open_output_file
        open(unit = 69, file = outputFilename, form = 'formatted', status = 'replace', action = 'write')
    end subroutine open_output_file


    ! Gets the size of the input file
    subroutine get_file_size
        inquire(file = FileName, size = fileSize)
    end subroutine get_file_size


    ! Reads data from file and calculates number of syllables, words, and sentences
    subroutine analyze_file (NumSyll, NumWord, NumSent)
        implicit none
        character (len=200) :: input
        integer, intent(out) :: NumSyll, NumWord, NumSent
        integer :: io
        NumSyll = 0
        NumWord = 0
        NumSent = 0
        io = 0

        ! Loop through lines in file until and invalid (EOF) line is read
        do while (io == 0) 
            read (69, '(a)', iostat = io) input
            if (io /= 0) then
                exit
            end if
            call parse_line(input, NumSyll, NumWord, NumSent)
        end do
    end subroutine analyze_file


    ! Parse an input line for number of syllables, words, and sentences
    subroutine parse_line (Input, NumberSyll, NumberWord, NumberSent)
        implicit none
        character, intent(in) :: Input(200)
        integer, intent(inout) :: NumberSyll, NumberWord, NumberSent
        integer :: wordStart, wordEnd
        logical :: startAgain

        ! Find start and end elements for each word, and test that word syllables and sentence terminators
        do wordStart = 1, 199
            if ((Input(wordStart) /= ' ') .and. (startAgain)) then
                startAgain = .false.
                do wordEnd = wordStart, 199
                    ! Gets parameters from word after end is found
                    if ((Input(wordEnd) == ' ') .or. (wordEnd == 199)) then
                        NumberSyll = NumberSyll + get_syll(Input, wordStart, wordEnd)
                        NumberSent = NumberSent + get_sent(Input, wordEnd)
                        NumberWord = NumberWord + 1
                        exit
                    end if
                end do
            end if
            if (wordStart == wordEnd) then
                startAgain = .true.
            end if
        end do
    end subroutine parse_line


    ! Returns the number of syllables in a word
    integer function get_syll (InputLine, WordStart, WordEnd)
        implicit none
        character, intent(in) :: InputLine(200)
        integer, intent(in) :: WordStart, WordEnd
        integer :: i, j, k, length, count
        count = 0

        do i = WordStart, WordEnd
            ! Test for double vowels
            if ((InputLine(i) == 'a') .or. (InputLine(i) == 'A') .or. (InputLine(i) == 'e') &
                .or. (InputLine(i) == 'E') .or. (InputLine(i) == 'i') .or. (InputLine(i) == 'I') &
                .or. (InputLine(i) == 'o') .or. (InputLine(i) == 'O') .or. (InputLine(i) == 'u') &
                .or. (InputLine(i) == 'U') .or. (InputLine(i) == 'y') .or. (InputLine(i) == 'Y')) then

                j = i + 1
                if ((InputLine(j) == 'a') .or. (InputLine(j) == 'A') .or. (InputLine(j) == 'e') &
                    .or. (InputLine(j) == 'E') .or. (InputLine(j) == 'i') .or. (InputLine(j) == 'I')&
                    .or. (InputLine(j) == 'o') .or. (InputLine(j) == 'O') .or. (InputLine(j) == 'u')&
                    .or. (InputLine(j) == 'U') .or. (InputLine(j) == 'y') .or. (InputLine(j) == 'Y')) then
                else
                    count = count + 1
                end if
            end if

            ! Test for special E cases
            if ((InputLine(i) == 'e') .or. (InputLine(i) == 'E')) then
                j = i + 1
                k = j + 1

                ! Test for -LE end of word
                if (InputLine(j) == ' ') then
                    j = i - 1
                    if ((InputLine(j) /= 'l') .and. (InputLine(j) /= 'L')) then
                        count = count - 1
                    end if
                end if

                ! Test for -ES and -ED cases
                if (InputLine(k) == ' ') then
                    j = i + 1
                    if ((InputLine(j) == 'd') .or. (InputLine(j) == 'D') .or. (InputLine(j) == 's')&
                        .or. (InputLine(j) == 's')) then
                        count = count - 1
                    end if
                end if
            end if
        end do

        ! Test for short word or no syllables
        length = WordEnd - WordStart
        if ((count < 1) .or. (length <= 3)) then
            count = 1
        end if

        get_syll = count
    end function get_syll


    ! Returns 1 if a sentence is terminated at the end of a word
    integer function get_sent (InputLine, WordEnd)
        implicit none
        character, intent(in) :: InputLine(200)
        integer, intent(in) :: WordEnd
        integer :: i, count
        i = WordEnd - 1
        count = 0

        if ((InputLine(i) == '.') .or. (InputLine(i) == '!') .or. (InputLine(i) == '?')) then
            count = 1
        end if
        get_sent = count
    end function get_sent


    ! Calculates the results
    subroutine calculate_output
        fleschIndex = 206.835 - (1.015 * numWord / numSent) - (84.6 * numSyll / numWord)
        gradeLevel = (0.39 * numWord / numSent) + (11.8 * numSyll / numWord) - 15.59
    end subroutine calculate_output


    ! Outputs the results to user's designated output file
    subroutine output_results 
        write (69,*) '========================================'
        write (69,*) '   Analysis results from ', trim(filename)
        write (69,*) '========================================'
        write (69,*) ' '
        write (69,*) 'Number of words:            ', numWord
        write (69,*) 'Number of syllables:        ', numSyll
        write (69,*) 'Number of sentences:        ', numSent
        write (69,*) ' '
        write (69,*) 'Flesch readability index:  ', fleschIndex
        write (69,*) 'Flesch-Kincaid grade level:', gradeLevel

        write (*,*) 'Results have been saved in ', trim(outputFilename)
    end subroutine output_results


    ! Closes the opened file
    subroutine close_file
        close(unit = 69)
    end subroutine close_file


    ! Displays error message if input file does not exist
    subroutine no_file
        write (*,*) 'File ', trim(filename), ' does not exist'
    end subroutine no_file

    ! Closing message to user when program ends
    subroutine end_program
        write (*,*) ' '
        write (*,*) 'Thank you for using the Flesch Index Calculator'
        write (*,*) ' '
    end subroutine end_program

end program flesch_calculator
