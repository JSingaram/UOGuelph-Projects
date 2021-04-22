package rogue;

/**
* Signals that a user's move is invalid and was not carried out.
*
*/
public class NoSuchItemException extends Exception {


    /**
    * Constructs an InvalidMoveException with "That is an invalid move." as its error detail message.
    *
    */
    public NoSuchItemException() {
        super("This item does not exist.");
    }


    /**
    * Constructs an InvalidMoveException with input String as its error detail message.
    * @param str String containing error message
    *
    */
    public NoSuchItemException(String str) {
        super(str);
    }
}
