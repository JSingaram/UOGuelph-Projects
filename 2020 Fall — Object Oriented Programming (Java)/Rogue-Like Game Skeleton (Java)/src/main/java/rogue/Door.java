package rogue;

import java.util.ArrayList;

/**
 * A Door in a room. Connects to another Room.
 */
public class Door {
    private ArrayList<Room> connectedRooms = new ArrayList<Room>();
    private Character direction;
    private int position;
    private ArrayList<Integer> connectedRoomsId = new ArrayList<Integer>();


    /**
    * Initializes a newly created Door with blank ArrayList of connectedRooms, assigns
    * direction as X, and assigns position as -1.
    *
    */
    public Door() {
        connectedRooms = new ArrayList<Room>();
        direction = 'X';
        position = -1;
    }


    /**
    * Initializes a newly created Door with input direction, position, and connected room.
    * @param newDirection The character representing the door's direction (N/E/S/W)
    * @param newPosition The integer representing the door's location on the wall
    *
    */
    public Door(Character newDirection, int newPosition) {
        connectedRooms = new ArrayList<Room>();
        setDirection(newDirection);
        position = newPosition;
    }


    /**
    * Adds Room passed as parameter to ArrayList of connectedRooms.
    * @param r The room to the added as a connected room
    *
    */
    public void connectRoom(Room r) {
        if (r != null) {
            connectedRooms.add(r);
        }
    }


    /**
    * Adds room id passed as parameter to ArrayList of connectedRoomsId.
    * @param newId The id of the room that will be connected
    *
    */
    public void connectRoomId(int newId) {
        connectedRoomsId.add(newId);
    }


    /**
    * Returns an ArrayList of integer ids of connected rooms.
    * @return ArrayList of integer ids of connected rooms
    *
    */
    public ArrayList<Integer> getConnectedRoomsId() {
        return connectedRoomsId;
    }


    /**
    * Returns the position of the door as an int.
    * @return The position of the door
    *
    */
    public int getPosition() {
        return position;
    }


    /**
    * Sets the position for the door as int.
    * @param newPostion The new position for the door
    *
    */
    public void setPosition(int newPostion) {
        position = newPostion;
    }


    /**
    * Returns the direction of the door as Character.
    * @return The direction of the door
    *
    */
    public Character getDirection() {
        return direction;
    }


    /**
    * Sets the direction of the door as Character.
    * @param newDirection The new direction for the door
    *
    */
    public void setDirection(Character newDirection) {
        direction = newDirection;
    }


    /**
    * Returns ArrayList of Rooms that are connected to the door.
    * @return ArrayList of Rooms that are connected to the door
    *
    */
    public ArrayList<Room> getConnectedRooms() {
        return connectedRooms;
    }


    /**
    * Takes a Room that is connected to door and returns to other connected door.
    * @param currentRoom Room that is conencted to door that is not being searched for
    * @return Room that is connected to door that is also not the input room
    *
    */
    public Room getOtherRoom(Room currentRoom) {
        // may need to use (currentRoom.getId() == connectedRooms.get(0).getId()) for if
        if (currentRoom == connectedRooms.get(0)) {
            return connectedRooms.get(1);
        } else {
            return connectedRooms.get(0);
        }
    }
}
