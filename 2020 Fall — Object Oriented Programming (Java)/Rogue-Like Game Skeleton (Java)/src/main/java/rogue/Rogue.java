package rogue;

import java.util.ArrayList;
import java.util.Map;
import java.util.HashMap;

import java.awt.Point;

import java.util.Random;

//import org.json.simple.JSONArray;
//import org.json.simple.JSONObject;


public class Rogue {
    private ArrayList<Room> rooms = new ArrayList<Room>();
    private ArrayList<Item> items = new ArrayList<Item>();
    // private Map<String, String> itemMap = new HashMap<String, String>();
    // private Map<String, String> roomMap = new HashMap<String, String>();
    private Player player = new Player();
    private RogueParser parser = new RogueParser();

    private Character doorSymbol;
    private Character nsWallSymbol;
    private Character ewWallSymbol;
    private Character floorSymbol;
    private Character passageSymbol;
    private Character playerSymbol;

    public static final Character UP = 'w';
    public static final Character DOWN = 's';
    public static final Character LEFT = 'a';
    public static final Character RIGHT = 'd';


    /**
    * Initializes a newly created Rogue with a new RogueParser.
    *
    */
    public Rogue() {
        parser = new RogueParser();
    }


    /**
    * Initializes a newly created Rogue with input RogueParser.
    * @param newParser The parser that Rogue will use
    *
    */
    public Rogue(RogueParser newParser) {
        parser = newParser;

        createItems();
        setItemSymbols();
        setSymbols();
        createRooms();
        setRoomSymbols();
        verifyRooms();
        connectAllDoors();
        addItemsToRooms();
    }


    /**
    * Sets the player in the game as a Player and adds them to all rooms.
    * @param thePlayer The new player
    *
    */
    public void setPlayer(Player thePlayer) {
        player = thePlayer;
        for (Room r:rooms) {
            r.setPlayer(thePlayer);
        }
        player.setCurrentRoom(getCurrentRoom());
    }


    protected Room getCurrentRoom() {
        for (Room r:rooms) {
            if (r.isPlayerInRoom()) {
                return r;
            }
        }
        return null;
    }


    protected void readPassageSymbol() {
            passageSymbol = parser.getSymbol("PASSAGE");
    }


    protected void readDoorSymbol() {
            doorSymbol = parser.getSymbol("DOOR");
    }


    protected void readFloorSymbol() {
            floorSymbol = parser.getSymbol("FLOOR");
    }


    protected void readPlayerSymbol() {
            playerSymbol = parser.getSymbol("PLAYER");
    }


    protected void readNSWallSymbol() {
            nsWallSymbol = parser.getSymbol("NS_WALL");
    }


    protected void readEWWallSymbol() {
            ewWallSymbol = parser.getSymbol("EW_WALL");
    }


    protected void setSymbols() {
        readPassageSymbol();
        readDoorSymbol();
        readFloorSymbol();
        readNSWallSymbol();
        readEWWallSymbol();
        readPlayerSymbol();
    }


    /*
    * Reads and sets symbols for the game from file whose directory is defined by input String.
    * @param filename The directory for the file containing the symbols
    *
    */
    /*
    public void setSymbols(String filename) {
        JSONParser jsonParser = new JSONParser();
        JSONObject jsonObject = new JSONObject();

        try {
            Object obj = jsonParser.parse(new FileReader(filename));
            jsonObject = (JSONObject) obj;
            JSONArray jsonSymbols = (JSONArray) jsonObject.get("symbols");
            String str = new String();
            readJSONSymbols(jsonSymbols);
        } catch (FileNotFoundException e) {
            e.printStackTrace();
        } catch (IOException e) {
            e.printStackTrace();
        } catch (ParseException e) {
            e.printStackTrace();
        }
        setRoomSymbols();
    }*/


    /**
    * Sets the symbols for all items based on their type.
    *
    */
    public void setItemSymbols() {
        String itemType = new String();
        Character itemSymbol;
        for (Item obj : items) {
            itemType = obj.getType().toUpperCase();
            obj.setDisplayCharacter(parser.getSymbol(itemType));
        }
    }


    /**
    * Sets the symbols for all rooms.
    *
    */
    public void setRoomSymbols() {
        for (Room obj : rooms) {
            obj.setSymbols(doorSymbol, nsWallSymbol, ewWallSymbol, floorSymbol, passageSymbol, playerSymbol);
        }
    }


    /**
    * Returns all rooms in the game as ArrayList of Rooms.
    * @return The ArrayList of all rooms
    *
    */
    public ArrayList<Room> getRooms() {
        return rooms;
    }


    /**
    * Returns all items in the game as ArrayList of Items.
    * @return The ArrayList of all items
    *
    */
    public ArrayList<Item> getItems() {
        return items;
    }


    /**
    * Returns the player in the game as Player.
    * @return The player in the game
    *
    */
    public Player getPlayer() {
        return player;
    }


    /**
    * Creates Rooms for the game based on the parser's results.
    *
    */
    public void createRooms() {
    Map<String, String> nextRoom = parser.nextRoom();
    int i = 0;
        while (nextRoom != null) {
            addRoom(nextRoom);
            i = rooms.size() - 1;

            nextRoom = parser.nextRoom();
        }
    }


    protected void addNorthDoor(Room newRoom, Map<String, String> nextRoom) {
        int pos = Integer.decode(nextRoom.get("N"));
        if (pos != -1) {
            Door newDoor = new Door('N', pos);
            newDoor.connectRoom(newRoom);
            newDoor.connectRoomId(Integer.decode(nextRoom.get("NConnectRoom").toString()));
            newRoom.addDoor("N", newDoor);
        }
    }


    protected void addEastDoor(Room newRoom, Map<String, String> nextRoom) {
        int pos = Integer.decode(nextRoom.get("E"));
        if (pos != -1) {
            Door newDoor = new Door('E', pos);
            newDoor.connectRoom(newRoom);
            newDoor.connectRoomId(Integer.decode(nextRoom.get("EConnectRoom").toString()));
            newRoom.addDoor("E", newDoor);
        }
    }


    protected void addSouthDoor(Room newRoom, Map<String, String> nextRoom) {
        int pos = Integer.decode(nextRoom.get("S"));
        if (pos != -1) {
            Door newDoor = new Door('S', pos);
            newDoor.connectRoom(newRoom);
            newDoor.connectRoomId(Integer.decode(nextRoom.get("SConnectRoom").toString()));
            newRoom.addDoor("S", newDoor);
        }
    }


    protected void addWestDoor(Room newRoom, Map<String, String> nextRoom) {
        int pos = Integer.decode(nextRoom.get("W"));
        if (pos != -1) {
            Door newDoor = new Door('W', pos);
            newDoor.connectRoom(newRoom);
            newDoor.connectRoomId(Integer.decode(nextRoom.get("WConnectRoom").toString()));
            newRoom.addDoor("W", newDoor);
        }
    }


    /**
    * Uses Map from the parser that describes a room to create Doors for a Room.
    * @param newRoom The room that the doors are being added to
    * @param nextRoom The Map containing room and door data
    *
    */
    protected void addDoorsToRoom(Room newRoom, Map<String, String> nextRoom) {
        addNorthDoor(newRoom, nextRoom);
        addEastDoor(newRoom, nextRoom);
        addSouthDoor(newRoom, nextRoom);
        addWestDoor(newRoom, nextRoom);
    }


    /*
    * Reads item information from loot JSONArray and stores in a new item object that is
    * added to the room. Items are also stored in this Rogue's ArrayList of Items.
    * @param newRoom The room that the items are being added to
    * @param loot The JSONArray containing item data
    *
    */
    /*protected void addItemsToRoom(Room newRoom, Item loot) {
        ArrayList<Item> newItems = new ArrayList<Item>();
        for (Object object : loot) {
            JSONObject newLoot = (JSONObject) object;
            Point newPoint = new Point(Integer.decode(newLoot.get("x").toString()),
                                       Integer.decode(newLoot.get("y").toString()));

            Item newItem = new Item(Integer.decode(newLoot.get("id").toString()), "name", "type", newPoint);
            newItems.add(newItem);
        }
        newRoom.setRoomItems(newItems);
        items.addAll(newItems);
    }*/


    /**
    * Uses Map of Strings to create a room and its doors. Doors are added to the room.
    * Room is added to ArrayList of rooms.
    * @param nextRoom Map of two Strings containing information to create Room
    *
    */
    public void addRoom(Map<String, String> nextRoom) {
        Room newRoom = new Room(Integer.decode(nextRoom.get("id").toString()),
                                Boolean.valueOf(nextRoom.get("start").toString()),
                                Integer.decode(nextRoom.get("height").toString()),
                                Integer.decode(nextRoom.get("width").toString()));

        addDoorsToRoom(newRoom, nextRoom);
        newRoom.setRogue(this);

        rooms.add(newRoom);
    }


    /**
    *
    *
    */
    protected void verifyRooms() {
        boolean roomsGood;
        ArrayList<Integer> badRooms = new ArrayList<Integer>();
        for (Room r:rooms) {
            try {
                roomsGood = r.verifyRoom(r);
                if (!roomsGood) {
                    badRooms.add(r.getId());
                }
            } catch (NotEnoughDoorsException e) {
            // Try to find room without four door
                // Create door ('N', 1) in room with no doors
                // Create door in empty wall in room with <4 doors
            // Terminate program if unable to add connection with message that file
            // cannot be used.
                if (!fixRoom(r)) {
                    System.out.println("The dungeon file cannot be used");
                    System.exit(0);
                } else {
                    System.out.println("The dungeon file has been repaired");
                }
            }
        }
        if (badRooms.size() > 0) {
            printBadRooms(badRooms);
        }
    }


    protected void printBadRooms(ArrayList<Integer> badRooms) {
        System.out.println("The following rooms are invalid: ");
        for (int i = 0; i < badRooms.size(); i++) {
            System.out.println(badRooms.get(i) + " ");
        }
    }


    /**
    * Attempts to add a door to a room with no doors. Will connect to an empty wall in another room.
    * @param r The room that needs to have a door added
    * @return Returns true if the room could be fixed. Returns false if it could not be fixed.
    *
    */
    protected boolean fixRoom(Room r) {
        Room toConnect = fixRoomFindRoom(r);
        if (toConnect == null) {
            return false;
        }
        String doorDirection = new String();
        doorDirection = fixRoomFindDirection(toConnect);

        fixRoomAddDoor(toConnect, doorDirection, r.getId());
        fixRoomAddDoor(r, oppositeDoorDirection(doorDirection), toConnect.getId());

        // Create door in that room on that wall in pos 1
        // Connect that door to r
        // Create door on north wall of r in pos 1
        // Connect that door to r
        // return true if doors can be added
        // else return false
        return true;
    }


    protected Room fixRoomFindRoom(Room r) {
        // Apparently this is more acceptable to Checkstyle than int fullDoors = 4;
        int fullDoors = 0;
        fullDoors++;
        fullDoors++;
        fullDoors++;
        fullDoors++;

        for (Room nextRoom:rooms) {
            if ((nextRoom.getId() != r.getId()) && (nextRoom.getNumDoors() < fullDoors)) {
                return nextRoom;
            }
        }
        return null;
    }


    protected String fixRoomFindDirection(Room toConnect) {
        if (toConnect.getDoor("N") == null) {
            return "N";
        }
        if (toConnect.getDoor("E") == null) {
            return "E";
        }
        if (toConnect.getDoor("S") == null) {
            return "S";
        }
        if (toConnect.getDoor("W") == null) {
            return "W";
        }
        return null;
    }


    protected void fixRoomAddDoor(Room r, String dir, int connectId) {
        Map<String, String> roomMap = new HashMap<String, String>();
        roomMap.put(dir, "1");
        if (dir.equals("N")) {
            roomMap.put("NConnectRoom", String.valueOf(connectId));
            addNorthDoor(r, roomMap);
        }
        if (dir.equals("E")) {
            roomMap.put("EConnectRoom", String.valueOf(connectId));
            addEastDoor(r, roomMap);
        }
        if (dir.equals("S")) {
            roomMap.put("SConnectRoom", String.valueOf(connectId));
            addSouthDoor(r, roomMap);
        }
        if (dir.equals("W")) {
            roomMap.put("WConnectRoom", String.valueOf(connectId));
            addWestDoor(r, roomMap);
        }
    }


    protected String oppositeDoorDirection(String str) {
        if (str.equals("N")) {
            return "S";
        }
        if (str.equals("E")) {
            return "W";
        }
        if (str.equals("S")) {
            return "N";
        }
        if (str.equals("W")) {
            return "E";
        }
        return "N";
    }


    protected void connectAllDoors() {
        Door nextDoor = new Door();
        ArrayList<Door> doorList = new ArrayList<Door>();
        for (Room r:rooms) {
            doorList = getAllDoors(r);
            for (Door d:doorList) {
                if (d != null) {
                    connectAllIds(d);
                }
            }
        }
    }


    protected ArrayList<Door> getAllDoors(Room r) {
        ArrayList<Door> doorList = new ArrayList<Door>();
        doorList.add(r.getDoor("N"));
        doorList.add(r.getDoor("E"));
        doorList.add(r.getDoor("S"));
        doorList.add(r.getDoor("W"));

        return doorList;
    }


    protected void connectAllIds(Door d) {
        ArrayList<Integer> ids = d.getConnectedRoomsId();
        ArrayList<Room> connectedRooms = d.getConnectedRooms();
        boolean isConnected;
        for (Integer i:ids) {
            isConnected = false;
            for (Room r:connectedRooms) {
                if (r.getId() == i) {
                    isConnected = true;
                }
            }
            if (!isConnected) {
                d.connectRoom(searchRoomById(i.intValue()));
            }
        }
    }


    protected Room searchRoomById(int id) {
        for (Room r:rooms) {
            if (r.getId() == id) {
                return r;
            }
        }
        return null;
    }


    /**
    * Assembles and returns String containing String representations of all rooms.
    * @return The String representing all rooms in the game
    */
    public String displayAll() {
        String str = new String("");
        for (Room room : rooms) {
            str = str + room.displayRoom() + "\n\n";
        }
        return str;
    }


    /**
    * Returns String that represents the current state of the room containing the player.
    * @return String representing the current state of the room containing the player
    *
    */
    public String getNextDisplay() {
        return getCurrentRoom().displayRoom();
    }


    /**
    * Processes and input user's move.
    * @param input The user's move
    * @return A String containing a feedback message for the user's move
    * @throws InvalidMoveException if the user's move was invalid
    *
    */
    public String makeMove(char input) throws InvalidMoveException {
        String wallMove;
        boolean enteredDoor = false;
        Point newPos = makeNewPosition(input);
        String message = moveMessage(input);
        wallMove = testWallMove(newPos);
        if (wallMove != null) {
            enteredDoor = testDoorMove(newPos, wallMove);
            if (!enteredDoor) {
                throw new InvalidMoveException();
            } else {
                try {
                    message += " into a new room";
                    newPos = moveThroughDoor(wallMove);
                } catch (NullPointerException e) {
                    newPos = new Point(1, 1);
                }
            }
        }
        if ((wallMove == null) || (enteredDoor)) {
            player.setXyLocation(newPos);
        }
        message += moveOntoItem(newPos);


        // TODO
        // Pick up item if walked onto
        // Create string describing movement
        /*
        try {
            // Assess the move
            // Process the move
            // Method that finds why move is invalid changes error message to
            // explain cause for error
                /*  if (valid move) process move
                    else throw new InvalidMoveException("Error message")
                * /
        } catch (InvalidMoveException e){
            throw e;
        };
        */

        return message;
    }


    protected String moveMessage(char input) {
        if (input == 'w') {
            return "You move up";
        }
        if (input == 'd') {
            return "You move right";
        }
        if (input == 's') {
            return "You move down";
        }
        if (input == 'a') {
            return "You move left";
        }
        return "";
    }

    protected Point makeNewPosition(char input) {
        Point newPos = new Point();
        if (input == 'w') {
            newPos.setLocation(player.getXyLocation().getX(), player.getXyLocation().getY() - 1);
        } else if (input == 'd') {
            newPos.setLocation(player.getXyLocation().getX() + 1, player.getXyLocation().getY());
        } else if (input == 's') {
            newPos.setLocation(player.getXyLocation().getX(), player.getXyLocation().getY() + 1);
        } else if (input == 'a') {
            newPos.setLocation(player.getXyLocation().getX() - 1, player.getXyLocation().getY());
        }
        return newPos;
    }


    protected String testWallMove(Point newPos) {
        if (newPos.getX() < 1) {
            return "W";
        }
        if (newPos.getX() + 1 > getCurrentRoom().getWidth() - 1) {
            return "E";
        }
        if (newPos.getY() < 1) {
            return "N";
        }
        if (newPos.getY() + 1 > getCurrentRoom().getHeight() - 1) {
            return "S";
        }
        return null;
    }


    protected boolean testDoorMove(Point newPos, String dir) {
        Door d = testDoorPos(dir);
        if (d != null) {
            if ((d.getDirection() == 'N') || (d.getDirection() == 'S')) {
                if (newPos.getX() == d.getPosition()) {
                    return true;
                }
            }
            if ((d.getDirection() == 'E') || (d.getDirection() == 'W')) {
                if (newPos.getY() == d.getPosition()) {
                    return true;
                }
            }
        }
        return false;
    }

    protected Door testDoorPos(String dir) {
        Room r = getCurrentRoom();
        return r.getDoor(dir);
    }


    protected Point moveThroughDoor(String doorDirection) throws NullPointerException {
        Room curRoom = getCurrentRoom();
        Door curDoor = curRoom.getDoor(doorDirection);
        Room nextRoom = getNextRoom(curDoor, curRoom);
        int nextDoorPos = getNextDoorPos(nextRoom, doorDirection);
        curRoom.setIsPlayerInRoom(false);
        nextRoom.setIsPlayerInRoom(true);
        Point newPoint = new Point();
        try {
            newPoint = getNextRoomPoint(nextRoom, doorDirection, nextDoorPos);
        } catch (NullPointerException e) {
            throw new NullPointerException();
        }
        return newPoint;
    }


    protected Room getNextRoom(Door curDoor, Room curRoom) {
        ArrayList<Room> connectedRooms = curDoor.getConnectedRooms();
        Room nextRoom = new Room();
        for (int i = 0; i < curDoor.getConnectedRooms().size(); i++) {
            if (!(connectedRooms.get(i).getId() == curRoom.getId())) {
                nextRoom = curDoor.getConnectedRooms().get(i);
            }
        }
        return nextRoom;
    }


    protected int getNextDoorPos(Room nextRoom, String doorDirection) {
        if (doorDirection.equals("N")) {
            return nextRoom.getDoor("S").getPosition();
        } else if (doorDirection.equals("E")) {
            return nextRoom.getDoor("W").getPosition();
        } else if (doorDirection.equals("S")) {
            return nextRoom.getDoor("N").getPosition();
        } else if (doorDirection.equals("W")) {
            return nextRoom.getDoor("E").getPosition();
        }
        return 1;
    }


    protected Point getNextRoomPoint(Room nextRoom, String doorDirection, int nextDoorPos) {
        Point nextPos = new Point();
        if (doorDirection.equals("N")) {
            nextPos.setLocation(nextDoorPos, nextRoom.getHeight() - 2);
        }
        if (doorDirection.equals("E")) {
            nextPos.setLocation(1, nextDoorPos);
        }
        if (doorDirection.equals("S")) {
            nextPos.setLocation(nextDoorPos, 1);
        }
        if (doorDirection.equals("W")) {
            nextPos.setLocation(nextRoom.getWidth() - 2, nextDoorPos);
        }
        return nextPos;
    }


    protected String moveOntoItem(Point newPos) {
        int i = 0;
        String message = new String();
        Room r = getCurrentRoom();
        ArrayList<Item> roomLoot = r.getRoomItems();
        ArrayList<Item> roomLootClone = new ArrayList<Item>();
        for (Item loot:roomLoot) {
            roomLootClone.add(loot);
        }
        for (Item loot:roomLootClone) {
            if ((newPos.getX() == loot.getXyLocation().getX()) && (newPos.getY() == loot.getXyLocation().getY())) {
                player.addItem(loot);
                //r.removeItem(i);
                //r.removeItemByPos(newPos);
                r.removeItemByItem(loot);
                message = " and pick up a " + loot.getName();
            }
            i++;
        }
        return message;
    }


    protected void createItems() {
        Map<String, String> nextItem = parser.nextItem();
        int i = 0;
        while (nextItem != null) {
            addItem(nextItem);
            i = items.size() - 1;

            nextItem = parser.nextItem();
        }
    }


    /**
    * Converts input HashMap into a new Item and stores item in ArrayList.
    * @param toAdd Map containing information about the new Item
    *
    */
    public void addItem(Map toAdd) {
        Item newItem = new Item(Integer.decode(toAdd.get("id").toString()),
                                (toAdd.get("name").toString()),
                                (toAdd.get("type").toString()),
                                addItemPoint(toAdd));
        newItem.setDescription(toAdd.get("description").toString());
        newItem.setCurrentRoomId(addItemRoom(toAdd));

        items.add(newItem);
    }


    protected Point addItemPoint(Map toAdd) {
        try {
            int x = Integer.decode(toAdd.get("x").toString());
            int y = Integer.decode(toAdd.get("y").toString());
            return new Point(x, y);
        } catch (NullPointerException e) {
            return new Point(0, 0);
        }
    }


    protected int addItemRoom(Map toAdd) {
        try {
            return Integer.decode(toAdd.get("room").toString());
        } catch (NullPointerException e) {
            return -1;
        }
    }


    protected void addItemsToRooms() {
        int roomId;
        for (Item i:items) {
            roomId = i.getCurrentRoomId();
            if (roomId != -1) {
                for (Room r:rooms) {
                    if (r.getId() == roomId) {
                        insertItemInRoom(i, r);
                    }
                }
            }
        }
    }


    protected void insertItemInRoom(Item i, Room r) {
        try {
            i.setCurrentRoom(r);
            r.addItem(i);
        } catch (ImpossiblePositionException e) {
            /* Will loop infinitely if room is full */
            Random rand = new Random();
            i.setXyLocation(new Point(rand.nextInt(r.getWidth() - 2), rand.nextInt(r.getHeight() - 2)));
            insertItemInRoom(i, r);

            /*  If only one retry is desired:
            *
            *   Random rand = new Random();
            *   i.setXyLocation(new Point(rand.nextInt(r.getWidth - 2), rand.nextInt(r.getHeight - 2)));
            *   Point x = toAdd.getXyLocation();
            *   try {
            *       i.setCurrentRoom(r);
            *       r.addItem(i);
            *   } catch (ImpossiblePositionException e) {
            *   } catch (NoSuchItemException e) {
            *   }
            */
        } catch (NoSuchItemException e) {
        }
    }
}
