package rogue;

/**
* Signals that a user's move is invalid and was not carried out.
*
*/
public class NotEnoughDoorsException extends Exception {


    /**
    * Constructs an InvalidMoveException with "That is an invalid move." as its error detail message.
    *
    */
    public NotEnoughDoorsException() {
        super("Room does not have enough doors.");
    }


    /**
    * Constructs an InvalidMoveException with input String as its error detail message.
    * @param str String containing error message
    *
    */
    public NotEnoughDoorsException(String str) {
        super(str);
    }
}
