#include <libxml/parser.h>
#include <libxml/tree.h>
#include <libxml/xmlschemastypes.h>
#include <math.h>
#include <stdlib.h>
#include "GPXParser.h"
#include "LinkedListAPI.h"


xmlDoc* getXMLDoc(char* fileName) {
    xmlDoc *doc = NULL;

    if (fileName == NULL) {
        return NULL;
    } else {
        doc = xmlReadFile(fileName, NULL, 0);
    }

    return doc;
}


bool printXMLFile(xmlDoc* doc, char* fileName) {
    LIBXML_TEST_VERSION
    if (xmlSaveFormatFileEnc(fileName, doc, NULL, 1) >= 0) {
        return true;
    } else {
        return false;
    }
}


List* makeWptList(){
    List *newList = malloc(sizeof(List));

    newList->head = NULL;
    newList->tail = NULL;
    newList->length = 0;
    newList->deleteData = deleteWaypoint;
    newList->compare = compareWaypoints;
    newList->printData = waypointToString;

    return newList;
}


List* makeRteList(){
    List *newList = malloc(sizeof(List));

    newList->head = NULL;
    newList->tail = NULL;
    newList->length = 0;
    newList->deleteData = deleteRoute;
    newList->compare = compareRoutes;
    newList->printData = routeToString;

    return newList;
}


List* makeTrkList(){
    List *newList = malloc(sizeof(List));

    newList->head = NULL;
    newList->tail = NULL;
    newList->length = 0;
    newList->deleteData = deleteTrack;
    newList->compare = compareTracks;
    newList->printData = trackToString;

    return newList;
}


List* makeGpxDataList(){
    List *newList = malloc(sizeof(List));

    newList->head = NULL;
    newList->tail = NULL;
    newList->length = 0;
    newList->deleteData = deleteGpxData;
    newList->compare = compareGpxData;
    newList->printData = gpxDataToString;

    return newList;
}


List* makeTrkSegList(){
    List *newList = malloc(sizeof(List));

    newList->head = NULL;
    newList->tail = NULL;
    newList->length = 0;
    newList->deleteData = deleteTrackSegment;
    newList->compare = compareTrackSegments;
    newList->printData = trackSegmentToString;

    return newList;
}

Node* makeNullListNode() {
    Node *newNode = malloc(sizeof(Node));

    newNode->data = NULL;
    newNode->previous = NULL;
    newNode->next = NULL;

    return newNode;
}


Waypoint* makeNullWaypoint() {
    Waypoint *newWaypoint = malloc(sizeof(Waypoint));

    newWaypoint->name = NULL;
    newWaypoint->longitude = 0.0;
    newWaypoint->latitude = 0.0;
    newWaypoint->otherData = NULL;

    return newWaypoint;
}


Route* makeNullRoute() {
    Route *newRoute = malloc(sizeof(Route));

    newRoute->name = NULL;
    newRoute->waypoints = NULL;
    newRoute->otherData = NULL;

    return newRoute;
}


TrackSegment* makeNullTrackSegment() {
    TrackSegment *newTrackSegment = malloc(sizeof(TrackSegment));

    newTrackSegment->waypoints = NULL;

    return newTrackSegment;
}


Track* makeNullTrack() {
    Track *newTrack = malloc(sizeof(Track));

    newTrack->name = NULL;
    newTrack->segments = NULL;
    newTrack->otherData = NULL;

    return newTrack;
}


void addGpxDataToList(List *theList, xmlNode *cur_node) {
    GPXData *gpxData = NULL;
    char *str = NULL;
    if ((theList == NULL) || (cur_node == NULL)) {
        return;
    }
    if ((strcmp ((char*) cur_node->name, "name") == 0) || (strcmp ((char*) cur_node->name, "wpt") == 0) || 
        (strcmp ((char*) cur_node->name, "rte") == 0) || (strcmp ((char*) cur_node->name, "trk") == 0)) {
        return;
    }

    str = (char*) xmlNodeGetContent(cur_node);
    gpxData = calloc(sizeof(GPXData) + strlen(str) + 1, 1);


    if (strlen((char*) cur_node->name)) {
        strncpy(gpxData->name, (char*) cur_node->name, 256);
    } else {
        strncpy(gpxData->name, "\0", 1);
    }
    if (strlen(str)) {
        strncpy(gpxData->value, str, strlen(str) + 1);
    } else {
        strncpy(gpxData->value, "\0", 1);
    }

    /* Add wpt to list */
    if ((theList->length == 0) && (theList->tail == NULL)) {
        theList->head = makeNullListNode();
        theList->tail = theList->head;
        theList->length++;
        theList->head->data = gpxData;
    } else {
        theList->tail->next = makeNullListNode();
        theList->tail->next->previous = theList->tail;
        theList->tail = theList->tail->next;
        theList->tail->data = gpxData;
        theList->length++;
    }

    if (str != NULL) {
        free(str);
    }
}


void addWptToList(List *theList, xmlNode *cur_node) {
    Waypoint *wpt = NULL;
    xmlNode *child_node = NULL;
    char *str = NULL;
    if ((theList == NULL) || (cur_node == NULL)) {
        return;
    }
    if ((strcmp ((char*) cur_node->name, "wpt")) && (strcmp ((char*) cur_node->name, "rtept")) &&
        (strcmp ((char*) cur_node->name, "trkpt"))) {
        return;
    }

    wpt = makeNullWaypoint();
    for (child_node = cur_node->children; child_node != NULL; child_node = child_node->next) {
        if (strcmp ((char*) child_node->name, "name") == 0) {
            str = (char*) xmlNodeGetContent(child_node);
            if (str != NULL) {
                wpt->name = malloc(strlen((char*) str) + 1);
                strcpy(wpt->name, (char*) str);
                free(str);
            } else {
                wpt->name = malloc(1);
                *(wpt->name) ='\0';
            }
        }
    }
    if (wpt->name == NULL) {
        wpt->name = malloc(1);
        *(wpt->name) ='\0';
    }

    /* Get lon */
    str = (char*) xmlGetProp(cur_node, (xmlChar*)"lon");
    if (str != NULL) {
        wpt->longitude = strtod(str, NULL);
        free(str);
    } else {
        wpt->longitude = 0.0;
    }
    

    /* Get lat */
    str = (char*) xmlGetProp(cur_node, (xmlChar*)"lat");
    if (str != NULL) {
        wpt->latitude = strtod(str, NULL);
        free(str);
    } else {
        wpt->latitude = 0.0;
    }
    

    wpt->otherData = makeGpxDataList();
    for (child_node = cur_node->children; child_node != NULL; child_node = child_node->next) {
        if (child_node->type == XML_ELEMENT_NODE){
            if (strcmp ((char*) child_node->name, "name")) {
                addGpxDataToList(wpt->otherData, child_node);
            }
        }
    }

    /* Add wpt to list */
    if ((theList->length == 0) && (theList->tail == NULL)) {
        theList->head = makeNullListNode();
        theList->tail = theList->head;
        theList->length++;
        theList->head->data = wpt;
    } else {
        theList->tail->next = makeNullListNode();
        theList->tail->next->previous = theList->tail;
        theList->tail = theList->tail->next;
        theList->tail->data = wpt;
        theList->length++;
    }
}


void addRteToList(List *theList, xmlNode *cur_node) {
    Route *rte = NULL;
    xmlNode *child_node = NULL;
    char *str = NULL;
    if ((theList == NULL) || (cur_node == NULL)) {
        return;
    }
    if (strcmp ((char*) cur_node->name, "rte")) {
        return;
    }

    rte = makeNullRoute();
    for (child_node = cur_node->children; child_node != NULL; child_node = child_node->next) {
        if (strcmp ((char*) child_node->name, "name") == 0) {
            str = (char*) xmlNodeGetContent(child_node);
            if (str != NULL) {
                rte->name = malloc(strlen((char*) str) + 1);
                strcpy(rte->name, (char*) str);
                free(str);
            } else {
                rte->name = malloc(1);
                *(rte->name) ='\0';
            }
        }
    }
    if (rte->name == NULL) {
        rte->name = malloc(1);
        *(rte->name) ='\0';
    }

    rte->waypoints = makeWptList();
    rte->otherData = makeGpxDataList();
    for (child_node = cur_node->children; child_node != NULL; child_node = child_node->next) {
        if (child_node->type == XML_ELEMENT_NODE){
            if (strcmp ((char*) child_node->name, "rtept") == 0) {
                addWptToList(rte->waypoints, child_node);\
            } else if (strcmp ((char*) child_node->name, "rtept") && (strcmp ((char*) child_node->name, "name"))) {
                addGpxDataToList(rte->otherData, child_node);
            }
        }
    }

    /* Add rte to list */
    if ((theList->length == 0) && (theList->tail == NULL)) {
        theList->head = makeNullListNode();
        theList->tail = theList->head;
        theList->length++;
        theList->head->data = rte;
    } else {
        theList->tail->next = makeNullListNode();
        theList->tail->next->previous = theList->tail;
        theList->tail = theList->tail->next;
        theList->tail->data = rte;
        theList->length++;
    }
}


void addTrkSegToList(List *theList, xmlNode *cur_node) {
    TrackSegment *trkSeg = NULL;
    xmlNode *child_node = NULL;
    if ((theList == NULL) || (cur_node == NULL)) {
        return;
    }
    if (strcmp ((char*) cur_node->name, "trkseg")) {
        return;
    }

    trkSeg = makeNullTrackSegment();

    trkSeg->waypoints = makeWptList();
    for (child_node = cur_node->children; child_node != NULL; child_node = child_node->next) {
        if (child_node->type == XML_ELEMENT_NODE){
            if (strcmp ((char*) child_node->name, "trkpt") == 0) {
                addWptToList(trkSeg->waypoints, child_node);\
            }
        }
    }

    /* Add trkSeg to list */ 
    if ((theList->length == 0) && (theList->tail == NULL)) {
        theList->head = makeNullListNode();
        theList->tail = theList->head;
        theList->length++;
        theList->head->data = trkSeg;
    } else {
        theList->tail->next = makeNullListNode();
        theList->tail->next->previous = theList->tail;
        theList->tail = theList->tail->next;
        theList->tail->data = trkSeg;
        theList->length++;
    }
}


void addTrkToList(List *theList, xmlNode *cur_node) {
    Track *trk = NULL;
    xmlNode *child_node = NULL;
    char *str = NULL;
    if ((theList == NULL) || (cur_node == NULL)) {
        return;
    }
    if (strcmp ((char*) cur_node->name, "trk")) {
        return;
    }

    trk = makeNullTrack();
    for (child_node = cur_node->children; child_node != NULL; child_node = child_node->next) {
        if (strcmp ((char*) child_node->name, "name") == 0) {
            str = (char*) xmlNodeGetContent(child_node);
            if (str != NULL) {
                trk->name = malloc(strlen((char*) str) + 1);
                strcpy(trk->name, (char*) str);
                free(str);
            } else {
                trk->name = malloc(1);
                *(trk->name) ='\0';
            }
        }
    }
    if (trk->name == NULL) {
        trk->name = malloc(1);
        *(trk->name) ='\0';
    }

    trk->segments = makeTrkSegList();
    trk->otherData = makeGpxDataList();
    for (child_node = cur_node->children; child_node != NULL; child_node = child_node->next) {
        if (child_node->type == XML_ELEMENT_NODE){
            if (strcmp ((char*) child_node->name, "trkseg") == 0) {
                addTrkSegToList(trk->segments, child_node);\
            } else if (strcmp ((char*) child_node->name, "trkseg") && (strcmp ((char*) child_node->name, "name"))) {
                addGpxDataToList(trk->otherData, child_node);
            }
        }
    }

    /* Add trk to list */
    if ((theList->length == 0) && (theList->tail == NULL)) {
        theList->head = makeNullListNode();
        theList->tail = theList->head;
        theList->length++;
        theList->head->data = trk;
    } else {
        theList->tail->next = makeNullListNode();
        theList->tail->next->previous = theList->tail;
        theList->tail = theList->tail->next;
        theList->tail->data = trk;
        theList->length++;
    }
}


GPXdoc* createGPXdoc(char* fileName) {
    xmlDoc *doc = NULL;
    GPXdoc *gpx_doc = NULL;
    xmlNode *cur_node = NULL;
    void *getProp = NULL;

    /* Test that file name is not null */
    if ((fileName == NULL) || (fileName[0] == '\0')) {
        return NULL;
    }

    LIBXML_TEST_VERSION
    xmlInitParser();
    doc = getXMLDoc(fileName);

    /* Test that xmlDoc* is created */
    if (doc == NULL) {
        xmlFreeDoc(doc);
        return NULL;
    } 

    cur_node = xmlDocGetRootElement(doc);

    if ((cur_node->type == XML_ELEMENT_NODE) && (strcmp((char*)cur_node->name, "gpx") == 0)) {
        gpx_doc = malloc(sizeof(GPXdoc));
        /* Get version */
        if ((getProp = xmlGetProp(cur_node, (xmlChar*)"version")) != NULL) {
            gpx_doc->version = strtod((char*) getProp, NULL);
            free(getProp);
        } else {
            gpx_doc->version = 0.0;
        }
        /* Get creator */
        if ((getProp = xmlGetProp(cur_node, (xmlChar*)"creator")) != NULL) {
            gpx_doc->creator = malloc(strlen((char*) getProp) + 1);
            strncpy(gpx_doc->creator, (char*) getProp, strlen((char*) getProp) + 1);
            free(getProp);
        } else {
            gpx_doc->creator = malloc(1);
            *(gpx_doc->creator) ='\0';
        }

        /* Get namespace */
        if (cur_node->ns != NULL) {
            if (cur_node->ns->href != NULL) {
                strncpy((gpx_doc->namespace), (char*) cur_node->ns->href, 256);
            }
        } else {
            strncpy((gpx_doc->namespace), "http://www.topografix.com/GPX/1/1", 256);
        }

        /* Allocate lists */
        gpx_doc->waypoints = makeWptList();
        gpx_doc->routes = makeRteList();
        gpx_doc->tracks = makeTrkList();

    } else {
        xmlFreeDoc(doc);
        return NULL;
    }

    /* Iterate through level one children nodes */
    for (cur_node = cur_node->children; cur_node != NULL; cur_node = cur_node->next) {
        if (cur_node->type == XML_ELEMENT_NODE){
            if (strcmp ((char*) cur_node->name, "wpt") == 0) {
                addWptToList(gpx_doc->waypoints, cur_node);
            }
            if (strcmp ((char*) cur_node->name, "rte") == 0) {
                addRteToList(gpx_doc->routes, cur_node);
            }
            if (strcmp ((char*) cur_node->name, "trk") == 0) {
                addTrkToList(gpx_doc->tracks, cur_node);
            }
        }
    }

    xmlFreeDoc(doc);
    return gpx_doc;
}


void addString(char **str, char *toAdd) {
    if ((*str == NULL) || (toAdd == NULL)) {
        return;
    }

    *str = realloc(*str, (strlen(*str) + strlen(toAdd) + 2));
    strcat(*str, "\t");
    strcat(*str, toAdd);
    return;
}


void addJSONString(char **str, char *toAdd) {
    if ((*str == NULL) || (toAdd == NULL)) {
        return;
    }

    *str = realloc(*str, (strlen(*str) + strlen(toAdd) + 1));
    strcat(*str, toAdd);
    return;
}

char* GPXdocToString(GPXdoc* doc) {
    if (doc == NULL) {
        return NULL;
    }
    char *str = malloc(16);
    char *strFromFunc;
    char temp[5];
    Node *cur_node;


    strncpy(str, "\tNode type: gpx", 16);
    addString(&str, "\n");
    addString(&str, "Namespace:");
    addString(&str, doc->namespace);
    addString(&str, "\n");

    addString(&str, "Version: ");
    sprintf(temp, "%2.1f", doc->version);
    addString(&str, temp);
    addString(&str, "\n");

    addString(&str, "Creator: ");
    addString(&str, doc->creator);
    addString(&str, "\n\n");

    for(cur_node = doc->waypoints->head; cur_node != NULL; cur_node = cur_node->next) {
        if (cur_node != NULL) {
            strFromFunc = (doc->waypoints->printData)(cur_node->data);
            addString(&str, strFromFunc);
            free(strFromFunc);
        }
    }
    cur_node = doc->waypoints->head;

    for(cur_node = doc->routes->head; cur_node != NULL; cur_node = cur_node->next) {
        if (cur_node != NULL) {
            strFromFunc = (doc->routes->printData)(cur_node->data);
            addString(&str, strFromFunc);
            free(strFromFunc);
        }
    }
    cur_node = doc->routes->head;

    for(cur_node = doc->tracks->head; cur_node != NULL; cur_node = cur_node->next) {
        if (cur_node != NULL) {
            strFromFunc = (doc->tracks->printData)(cur_node->data);
            addString(&str, strFromFunc);
            free(strFromFunc);
        }
    }
    cur_node = doc->tracks->head;
    

    return str;
}



void deleteGPXdoc(GPXdoc* doc) {
    if (doc != NULL) {
        if (doc->creator != NULL) {
            free(doc->creator);
        }
        if (doc->waypoints != NULL) {
            freeList((List*) doc->waypoints);
        }
        if (doc->routes != NULL) {
            freeList((List*) doc->routes);
        }
        if (doc->tracks != NULL) {
            freeList((List*) doc->tracks);
        }        
        free(doc);
    }
    xmlCleanupParser();

    return;
}



int getNumWaypoints(const GPXdoc* doc) {
    int numWpt = 0;
    if (doc == NULL) {
        return 0;
    }

    numWpt = doc->waypoints->length;

    return numWpt;
}



int getNumRoutes(const GPXdoc* doc) {
    int numRte = 0;
    if (doc == NULL) {
        return 0;
    }

    numRte = doc->routes->length;

    return numRte;
}



int getNumTracks(const GPXdoc* doc) {
    int numTrk = 0;
    if (doc == NULL) {
        return 0;
    }

    numTrk = doc->tracks->length;

    return numTrk;
}



int getNumSegments(const GPXdoc* doc) {
    int numSeg = 0;
    Node *cur_node = NULL;
    if (doc == NULL) {
        return 0;
    }

    for(cur_node = doc->tracks->head; cur_node != NULL; cur_node = cur_node->next) {
        numSeg += (int)(((Track*) cur_node->data)->segments->length);
    }

    return numSeg;
}


int getNumGPXDataGpx(List *theList) {
    int numGpxData = 0;
    Node *cur_node = NULL;
    if (theList == NULL){
        return 0;
    }

    for(cur_node = theList->head; cur_node != NULL; cur_node = cur_node->next) {
        numGpxData++;
    }

    return numGpxData;
}


int getNumGPXDataWpt(List *theList) {
    int numGpxData = 0;
    Node *cur_node = NULL;
    if (theList == NULL){
        return 0;
    }

    for(cur_node = theList->head; cur_node != NULL; cur_node = cur_node->next) {
        if (strcmp ((char*) ((Waypoint*) cur_node->data)->name, "\0")) {
            numGpxData++;
        }
        numGpxData += getNumGPXDataGpx(((Waypoint*) cur_node->data)->otherData);
    }

    return numGpxData;
}


int getNumGPXDataRte(List *theList) {
    int numGpxData = 0;
    Node *cur_node = NULL;
    if (theList == NULL){
        return 0;
    }

    for(cur_node = theList->head; cur_node != NULL; cur_node = cur_node->next) {
        if (strcmp ((char*) ((Route*) cur_node->data)->name, "\0")) {
            numGpxData++;
        }
        numGpxData += getNumGPXDataWpt(((Route*) cur_node->data)->waypoints);
        numGpxData += getNumGPXDataGpx(((Route*) cur_node->data)->otherData);
    }

    return numGpxData;
}


int getNumGPXDataTrkSeg(List *theList) {
    int numGpxData = 0;
    Node *cur_node = NULL;
    if (theList == NULL){
        return 0;
    }

    for(cur_node = theList->head; cur_node != NULL; cur_node = cur_node->next) {
        numGpxData += getNumGPXDataWpt(((TrackSegment*) cur_node->data)->waypoints);
    }

    return numGpxData;
}


int getNumGPXDataTrk(List *theList) {
    int numGpxData = 0;
    Node *cur_node = NULL;
    if (theList == NULL){
        return 0;
    }

    for(cur_node = theList->head; cur_node != NULL; cur_node = cur_node->next) {
        if (strcmp ((char*) ((Track*) cur_node->data)->name, "\0")) {
            numGpxData++;
        }
        numGpxData += getNumGPXDataTrkSeg(((Track*) cur_node->data)->segments);
        numGpxData += getNumGPXDataGpx(((Track*) cur_node->data)->otherData);
    }

    return numGpxData;
}


int getNumGPXData(const GPXdoc* doc) {
    int numGpxData = 0;
    if (doc == NULL) {
        return 0;
    }

    /*if (strcmp ((char*) doc->name, "\0")) {
        numGpxData++;
    }*/

    numGpxData += getNumGPXDataWpt(doc->waypoints);
    numGpxData += getNumGPXDataRte(doc->routes);
    numGpxData += getNumGPXDataTrk(doc->tracks);

    return numGpxData;
}



Waypoint* getWaypoint(const GPXdoc* doc, char* name) {
    Node *cur_node = NULL;
    if ((doc == NULL) || (name == NULL)) {
        return NULL;
    }

    for(cur_node = doc->waypoints->head; cur_node != NULL; cur_node = cur_node->next) {
        if (strcmp ((char*) ((Waypoint*) cur_node->data)->name, name) == 0) {
            return cur_node->data;
        }
    }
    return NULL;
}



Track* getTrack(const GPXdoc* doc, char* name) {
    Node *cur_node = NULL;
    if ((doc == NULL) || (name == NULL)) {
        return NULL;
    }

    for(cur_node = doc->tracks->head; cur_node != NULL; cur_node = cur_node->next) {
        if (strcmp ((char*) ((Track*) cur_node->data)->name, name) == 0) {
            return cur_node->data;
        }
    }
    return NULL;
}



Route* getRoute(const GPXdoc* doc, char* name) {
    Node *cur_node = NULL;
    if ((doc == NULL) || (name == NULL)) {
        return NULL;
    }

    for(cur_node = doc->routes->head; cur_node != NULL; cur_node = cur_node->next) {
        if (strcmp ((char*) ((Route*) cur_node->data)->name, name) == 0) {
            return cur_node->data;
        }
    }
    return NULL;
}



void deleteGpxData( void* data) {
    if ((GPXData*) data != NULL) {
        free((GPXData*) data);
    }

    return;
}



char* gpxDataToString( void* data) {
    if (data == NULL) {
        return NULL;
    }
    char *str = malloc(12);

    strncpy(str, "Node type: ", 12);
    addString(&str, ((GPXData*) data)->name);
    addString(&str, "\n");
    addString(&str, "Value:\t");
    addString(&str, ((GPXData*) data)->value);
    addString(&str, "\n\n");

    return str;
}



int compareGpxData(const void *first, const void *second) {
    if ((first == NULL) || (second == NULL)) {
        return -2;
    }
    if (first < second) {
        return -1;
    } else if (first == second) {
        return 0;
    } else {
        return 1;
    }

    return 0;
}



void deleteWaypoint(void* data) {
    if (data == NULL) {
        return;
    }
    if (((Waypoint*) data)->name != NULL) {
        free(((Waypoint*) data)->name);
    }
    if (((Waypoint*) data)->otherData != NULL) {
        freeList(((Waypoint*) data)->otherData);
    }
    if (((Waypoint*) data) != NULL) {
        free((Waypoint*) data);
    }

    return;
}



char* waypointToString( void* data) {
    if (data == NULL) {
        return NULL;
    }
    char *str = malloc(11);
    char temp[50];
    char *strFromFunc;
    Node *cur_node;

    strncpy(str, "Node type:", 11);
    addString(&str, "wpt/rtept/trkpt");
    addString(&str, "\n");
    addString(&str, "Name:\t");
    addString(&str, ((Waypoint*) data)->name);
    addString(&str, "\n");

    addString(&str, "Longitude: ");
    sprintf(temp, "%f", ((Waypoint*) data)->longitude);
    addString(&str, temp);
    addString(&str, "\n");

    addString(&str, "Latitude: ");
    sprintf(temp, "%f", ((Waypoint*) data)->latitude);
    addString(&str, temp);
    addString(&str, "\n\n");

    for(cur_node = ((Waypoint*) data)->otherData->head; cur_node != NULL; cur_node = cur_node->next) {
        if (cur_node != NULL) {
            strFromFunc = (((Waypoint*) data)->otherData->printData)(cur_node->data);
            addString(&str, strFromFunc);
            free(strFromFunc);
        }
    }

    return str;
}



int compareWaypoints(const void *first, const void *second) {
    if ((first == NULL) || (second == NULL)) {
        return -2;
    }
    if (first < second) {
        return -1;
    } else if (first == second) {
        return 0;
    } else {
        return 1;
    }

    return 0;
}



void deleteRoute(void* data) {
    if (data == NULL) {
        return;
    }
    if (((Route*) data)->name != NULL) {
        free(((Route*) data)->name);
    }
    if (((Route*) data)->waypoints != NULL) {
        freeList(((Route*) data)->waypoints);
    }
    if (((Route*) data)->otherData != NULL) {
        freeList(((Route*) data)->otherData);
    }
    if ((Route*) data != NULL) {
        free((Route*) data);
    }

    return;
}



char* routeToString(void* data) {
    if (data == NULL) {
        return NULL;
    }
    char *str = malloc(11);
    char *strFromFunc;
    Node *cur_node;

    strncpy(str, "Node type:", 11);
    addString(&str, "rte");
    addString(&str, "\n");
    addString(&str, "Name:\t");
    addString(&str, ((Route*) data)->name);
    addString(&str, "\n\n");
    
    for(cur_node = ((Route*) data)->otherData->head; cur_node != NULL; cur_node = cur_node->next) {
        if (cur_node != NULL) {
            strFromFunc = (((Route*) data)->otherData->printData)(cur_node->data);
            addString(&str, strFromFunc);
            free(strFromFunc);
        }
    }

    for(cur_node = ((Route*) data)->waypoints->head; cur_node != NULL; cur_node = cur_node->next) {
        if (cur_node != NULL) {
            strFromFunc = (((Route*) data)->waypoints->printData)(cur_node->data);
            addString(&str, strFromFunc);
            free(strFromFunc);
        }
    }

    return str;
}



int compareRoutes(const void *first, const void *second) {
    if ((first == NULL) || (second == NULL)) {
        return -2;
    }
    if (first < second) {
        return -1;
    } else if (first == second) {
        return 0;
    } else {
        return 1;
    }

    return 0;
}



void deleteTrackSegment(void* data) {
    if (data == NULL) {
        return;
    }
    if (((TrackSegment*) data)->waypoints != NULL) {
        freeList(((TrackSegment*) data)->waypoints);
    }
    if ((TrackSegment*) data != NULL) {
        free((TrackSegment*) data);
    }

    return;
}



char* trackSegmentToString(void* data) {
    if (data == NULL) {
        return NULL;
    }
    char *str = malloc(11);
    char *strFromFunc;
    Node *cur_node;

    strncpy(str, "Node type:", 11);
    addString(&str, "trkseg");
    addString(&str, "\n\n");

    for(cur_node = ((TrackSegment*) data)->waypoints->head; cur_node != NULL; cur_node = cur_node->next) {
        if (cur_node != NULL) {
            strFromFunc = (((TrackSegment*) data)->waypoints->printData)(cur_node->data);
            addString(&str, strFromFunc);
            free(strFromFunc);
        }
    }

    return str;
}



int compareTrackSegments(const void *first, const void *second) {
    if ((first == NULL) || (second == NULL)) {
        return -2;
    }
    if (first < second) {
        return -1;
    } else if (first == second) {
        return 0;
    } else {
        return 1;
    }

    return 0;
}



void deleteTrack(void* data) {
    if (data == NULL) {
        return;
    }
    if (((Track*) data)->name != NULL) {
        free(((Route*) data)->name);
    }
    if (((Track*) data)->segments != NULL) {
        freeList(((Track*) data)->segments);
    }
    if (((Track*) data)->otherData != NULL) {
        freeList(((Track*) data)->otherData);
    }
    if ((Track*) data != NULL) {
        free((Track*) data);
    }

    return;
}



char* trackToString(void* data) {
    if (data == NULL) {
        return NULL;
    }
    char *str = malloc(11);
    char *strFromFunc;
    Node *cur_node;

    strncpy(str, "Node type:", 11);
    addString(&str, "trk");
    addString(&str, "\n");
    addString(&str, "Name:\t");
    addString(&str, ((Track*) data)->name);
    addString(&str, "\n\n");
    
    for(cur_node = ((Track*) data)->otherData->head; cur_node != NULL; cur_node = cur_node->next) {
        if (cur_node != NULL) {
            strFromFunc = (((Track*) data)->otherData->printData)(cur_node->data);
            addString(&str, strFromFunc);
            free(strFromFunc);
        }
    }

    for(cur_node = ((Track*) data)->segments->head; cur_node != NULL; cur_node = cur_node->next) {
        if (cur_node != NULL) {
            strFromFunc = (((Track*) data)->segments->printData)(cur_node->data);
            addString(&str, strFromFunc);
            free(strFromFunc);
        }
    }

    return str;
}




int compareTracks(const void *first, const void *second) {
    if ((first == NULL) || (second == NULL)) {
        return -2;
    }
    if (first < second) {
        return -1;
    } else if (first == second) {
        return 0;
    } else {
        return 1;
    }

    return 0;
}



/*
 *  START OF A2 FUNCTIONS
 *
 */


void addGPXDataToDoc(void* data, xmlNodePtr root) {

    xmlNewChild(root, NULL, BAD_CAST ((GPXData*)data)->name, BAD_CAST ((GPXData*)data)->value);

    return;
}


void addWaypointToDoc(void* data, xmlNodePtr root) {
    xmlNodePtr cur_node = NULL;
    Node* listNode = NULL;
    char temp[10];
    
    if (strcmp ((char*) root->name, "rte") == 0) {
        cur_node = xmlNewChild(root, NULL, BAD_CAST "rtept", NULL);
    } else if (strcmp ((char*) root->name, "trkseg") == 0) {
        cur_node = xmlNewChild(root, NULL, BAD_CAST "trkpt", NULL);
    } else {
        cur_node = xmlNewChild(root, NULL, BAD_CAST "wpt", NULL);
    }

    if (((Waypoint*) data)->name[0] != '\0') {
        xmlNewChild(cur_node, NULL, BAD_CAST "name", (xmlChar*)((Waypoint*) data)->name);
    }
    

    sprintf(temp, "%2.5f", ((Waypoint*) data)->latitude);
    xmlNewProp(cur_node, (xmlChar*)"lat", (xmlChar*)temp);
    temp[0] = '\0';
    sprintf(temp, "%2.5f", ((Waypoint*) data)->longitude);
    xmlNewProp(cur_node, (xmlChar*) "lon", (xmlChar*)temp);

    if (((Waypoint*) data)->otherData != NULL) {
        listNode = ((Waypoint*) data)->otherData->head;
        while (listNode != NULL) {
            addGPXDataToDoc(listNode->data, cur_node);
            listNode = listNode->next;
        }
    }

    return;
}


void addRouteToDoc(void* data, xmlNodePtr root) {
    xmlNodePtr cur_node = NULL;
    Node* listNode = NULL;
    
    cur_node = xmlNewChild(root, NULL, BAD_CAST "rte", NULL);

    if (((Route*) data)->name[0] != '\0') {
        xmlNewChild(cur_node, NULL, BAD_CAST "name", (xmlChar*)((Route*) data)->name);
    }
    
    if (((Route*) data)->otherData != NULL) {
        listNode = ((Route*) data)->otherData->head;
        while (listNode != NULL) {
            addGPXDataToDoc(listNode->data, cur_node);
            listNode = listNode->next;
        }
    }

    if (((Route*) data)->waypoints != NULL) {
        listNode = ((Route*) data)->waypoints->head;
        while (listNode != NULL) {
            addWaypointToDoc(listNode->data, cur_node);
            listNode = listNode->next;
        }
    }

    return;
}


void addTrackSegmentToDoc(void* data, xmlNodePtr root) {
    xmlNodePtr cur_node = NULL;
    Node* listNode = NULL;
    
    cur_node = xmlNewChild(root, NULL, BAD_CAST "trkseg", NULL);

    if (((TrackSegment*) data)->waypoints != NULL) {
        listNode = ((TrackSegment*) data)->waypoints->head;
        while (listNode != NULL) {
            addWaypointToDoc(listNode->data, cur_node);
            listNode = listNode->next;
        }
    }

    return;
}


void addTrackToDoc(void* data, xmlNodePtr root) {
    xmlNodePtr cur_node = NULL;
    Node* listNode = NULL;
    
    cur_node = xmlNewChild(root, NULL, BAD_CAST "trk", NULL);


    if (((Track*) data)->name[0] != '\0') {
        xmlNewChild(cur_node, NULL, BAD_CAST "name", (xmlChar*)((Track*) data)->name);
    }
    
    if (((Track*) data)->otherData != NULL) {
        listNode = ((Track*) data)->otherData->head;
        while (listNode != NULL) {
            addGPXDataToDoc(listNode->data, cur_node);
            listNode = listNode->next;
        }
    }

    if (((Track*) data)->segments != NULL) {
        listNode = ((Track*) data)->segments->head;
        while (listNode != NULL) {
            addTrackSegmentToDoc(listNode->data, cur_node);
            listNode = listNode->next;
        }
    }

    return;
}


void addGPXDocToDoc(GPXdoc* doc, xmlNodePtr root) {
    Node* listNode = NULL;
    char temp[5];
    
    sprintf(temp, "%2.1f", doc->version);
    xmlNewProp(root, (xmlChar*)"version", (xmlChar*)temp);
    xmlNewProp(root, (xmlChar*) "creator", (xmlChar*)(doc->creator));

    if (doc->waypoints != NULL) {
        listNode = doc->waypoints->head;
        while (listNode != NULL) {
            addWaypointToDoc(listNode->data, root);
            listNode = listNode->next;
        }
    }

    if (doc->routes != NULL) {
        listNode = doc->routes->head;
        while (listNode != NULL) {
            addRouteToDoc(listNode->data, root);
            listNode = listNode->next;
        }
    }

    if (doc->tracks != NULL) {
        listNode = doc->tracks->head;
        while (listNode != NULL) {
            addTrackToDoc(listNode->data, root);
            listNode = listNode->next;
        }
    }

    return;
}


xmlDoc* gpxDocToXMLDoc(GPXdoc* gpxdoc) {
    xmlDoc* doc = NULL;
    xmlNodePtr root_node = NULL;
    xmlNsPtr ns = NULL;

    if (gpxdoc == NULL) {
        return NULL;
    }

    doc = xmlNewDoc(BAD_CAST "1.0");
    
    root_node = xmlNewNode(ns, BAD_CAST "gpx");
    ns = xmlNewNs(root_node, BAD_CAST (gpxdoc->namespace), NULL);
    xmlSetNs(root_node, ns);



    xmlDocSetRootElement(doc, root_node);
    addGPXDocToDoc(gpxdoc, root_node);

    return doc;
}


GPXdoc* createValidGPXdoc(char* fileName, char* gpxSchemaFile) {
    GPXdoc* doc = NULL;

    if ((fileName == NULL) || (fileName[0] == '\0') || 
        (gpxSchemaFile == NULL) || (gpxSchemaFile[0] == '\0')) {
        return NULL;
    }

    doc = createGPXdoc(fileName);

    if (doc == NULL) {
        return NULL;
    }

    if (validateGPXDoc(doc, gpxSchemaFile)) {
        return doc;
    } else {
        deleteGPXdoc(doc);
        return NULL;
    }
}


bool validateGPXDoc(GPXdoc* doc, char* gpxSchemaFile) {
    xmlDoc* xml_doc = NULL;
    xmlSchemaPtr schema = NULL;
    xmlSchemaParserCtxtPtr parserContext = NULL;
    xmlSchemaValidCtxtPtr validContext;

    if ((doc == NULL) || (gpxSchemaFile == NULL) || (gpxSchemaFile[0] == '\0')) {
        return false;
    }

    /* Convert GPXdoc into XMLtree */
    xml_doc = gpxDocToXMLDoc(doc);

    if (xml_doc == NULL) {
        return false;
    }

    parserContext = xmlSchemaNewParserCtxt(gpxSchemaFile);

    if (parserContext == NULL) {
        xmlFreeDoc(xml_doc);
        return false;
    }

    schema = xmlSchemaParse(parserContext);
    xmlSchemaFreeParserCtxt(parserContext);
    validContext = xmlSchemaNewValidCtxt(schema);


    if ((xmlSchemaValidateDoc(validContext, xml_doc)) == 0) {
        xmlSchemaFreeValidCtxt(validContext);
        xmlFreeDoc(xml_doc);
        xmlCleanupParser();
        return true;
    } else {
        xmlSchemaFreeValidCtxt(validContext);
        xmlFreeDoc(xml_doc);
        xmlCleanupParser();
        return false;
    }
}


bool writeGPXdoc(GPXdoc* doc, char* fileName) {
    xmlDoc* tempDoc = NULL;
    bool result = false;

    if ((doc == NULL) || (fileName == NULL) || (fileName[0] == '\0')) {
        return false;
    }

    tempDoc = gpxDocToXMLDoc(doc);
    result = printXMLFile(tempDoc, fileName);
    xmlFreeDoc(tempDoc);

    return result;
}


float round10(float len) {
    if (len < 0.0) {
        return 0.0;
    }
    len = len / 10.0;
    return (10.0 * round(len));
}


float toRad(float input) {
    return input * M_PI / 180;
}


float calcDistBetWpts(Waypoint* wpt1, Waypoint* wpt2) {
    long double radius = 6371000.0;
    long double lat1 = toRad(wpt1->latitude);
    long double lat2 = toRad(wpt2->latitude);
    long double lon1 = toRad(wpt1->longitude);
    long double lon2 = toRad(wpt2->longitude);
    long double deltaLat = fabs(lat1 - lat2);
    long double deltaLon = fabs(lon1 - lon2);
    
    long double a = sin(deltaLat / 2) * sin(deltaLat / 2) + 
              (cos(lat1) * cos(lat2) * sin(deltaLon / 2) * sin(deltaLon / 2));
    long double c = 2 * atan2(sqrt(a), sqrt(1-a));
    long double d = radius * c;

    return (float) d;
}


float getRouteLen(const Route *rt) {
    float totalDistance = 0.0;
    Node *cur_waypoint;
    if ((rt == NULL) || (rt->waypoints->length < 2)) {
        return 0.0;
    }

    if (rt->waypoints->head == NULL) {
        return totalDistance;
    }
    cur_waypoint = rt->waypoints->head;
    while(cur_waypoint->next != NULL) {
        totalDistance += calcDistBetWpts(cur_waypoint->data, cur_waypoint->next->data);
        cur_waypoint = cur_waypoint->next;
    }
    
    return totalDistance;
}


float getSegmentLen(const TrackSegment *seg) {
    float totalDistance = 0.0;
    Node *cur_waypoint;
    if ((seg == NULL) || (seg->waypoints->length < 2)) {
        return 0.0;
    }

    if (seg->waypoints->head == NULL) {
        return totalDistance;
    }
    cur_waypoint = seg->waypoints->head;
    while(cur_waypoint->next != NULL) {
        totalDistance += calcDistBetWpts(cur_waypoint->data, cur_waypoint->next->data);
        cur_waypoint = cur_waypoint->next;
    }
    
    return totalDistance;
}


float getTrackLen(const Track *tr) {
    float totalDistance = 0.0;
    Node *cur_segment;
    if (tr == NULL) {
        return 0.0;
    }

    if (tr->segments->head == NULL) {
        return totalDistance;
    }


    cur_segment = tr->segments->head;
    while(cur_segment != NULL) {
        totalDistance += getSegmentLen(cur_segment->data);
        if ((cur_segment->next) != NULL) {
            totalDistance += calcDistBetWpts(((TrackSegment*)cur_segment->data)->waypoints->tail->data, 
                                             ((TrackSegment*)cur_segment->next->data)->waypoints->head->data);
        }
        cur_segment = cur_segment->next;
    }
    
    return totalDistance;
}


int numRoutesWithLength(const GPXdoc* doc, float len, float delta) {
    Node *routeNode = NULL;
    float length;
    int count = 0;
    if ((doc == NULL) || (len < 0.0) || (delta < 0.0)) {
        return 0;
    }


    if (doc->routes->head == NULL) {
        return 0;
    }
    routeNode = doc->routes->head;
    while (routeNode != NULL) {
        length = getRouteLen(routeNode->data);
        if ((length > (len - delta)) && (length < (len + delta))) {
            count++;
        }
        routeNode = routeNode->next;
    }

    return count;
}


int numTracksWithLength(const GPXdoc* doc, float len, float delta) {
    Node *trackNode = NULL;
    float length;
    int count = 0;
    if ((doc == NULL) || (len < 0.0) || (delta < 0.0)) {
        return 0;
    }


    if (doc->tracks->head == NULL) {
        return 0;
    }
    trackNode = doc->tracks->head;
    while (trackNode != NULL) {
        length = getTrackLen(trackNode->data);
        if ((length > (len - delta)) && (length < (len + delta))) {
            count++;
        }
        trackNode = trackNode->next;
    }

    return count;
}


bool isLoopRoute(const Route* route, float delta) {
    bool isLoop = false;
    int numWpt = 0;
    Node *wptNode;
    if ((route == NULL) || (delta < 0.0)) {
        return false;
    }

    wptNode = route->waypoints->head;
    while (wptNode != NULL) {
        if (wptNode->data != NULL) {
            numWpt++;
        }
        wptNode = wptNode->next;
    }
    if ((calcDistBetWpts(route->waypoints->head->data, route->waypoints->tail->data) < delta) &&
       (numWpt >= 4)) {
        isLoop = true;
    }

    return isLoop;
}


bool isLoopTrack(const Track *tr, float delta) {
    bool isLoop = false;
    int numWpt = 0;
    Node *segNode;
    Node *wptNode;
    TrackSegment *seg;
    TrackSegment *firstSeg;
    TrackSegment *lastSeg;

    if ((tr == NULL) || (delta < 0.0)) {
        return false;
    }

    segNode = tr->segments->head;
    while (segNode != NULL) {
        if (segNode->data != NULL) {
            seg = segNode->data;
            wptNode = seg->waypoints->head;
            while (wptNode != NULL) {
                if (wptNode->data != NULL) {
                    numWpt++;
                }
                wptNode = wptNode->next;
            }
        }
        segNode = segNode->next;
    }

    firstSeg = tr->segments->head->data;
    lastSeg = tr->segments->tail->data;
    if ((calcDistBetWpts(firstSeg->waypoints->head->data, 
                         lastSeg->waypoints->tail->data)
        < delta) && (numWpt >= 4)) {
        isLoop = true;
    }

    return isLoop;
}


void dummyDelete(){
    return;
}


List* newRoutesBetweenList() {
    List *newList = malloc(sizeof(List));

    newList->head = NULL;
    newList->tail = NULL;
    newList->length = 0;
    newList->deleteData = dummyDelete;
    newList->compare = compareRoutes;
    newList->printData = routeToString;

    return newList;
}


List* getRoutesBetween(const GPXdoc* doc, float sourceLat, float sourceLong, float destLat, float destLong, float delta) {
    List* routeList = NULL;
    Node *routeNode = NULL, *newRouteNode = NULL;
    Route *curRoute = NULL;
    Waypoint startWpt;
    Waypoint endWpt;
    bool startMatch = false, endMatch = false;
    if (doc == NULL) {
        return NULL;
    }
    if (doc->routes->head == NULL) {
        return NULL;
    }

    startWpt.longitude = sourceLong;
    startWpt.latitude = sourceLat;
    endWpt.longitude = destLong;
    endWpt.latitude = destLat;

    routeList = newRoutesBetweenList();

    routeNode = doc->routes->head;
    while (routeNode != NULL) {
        curRoute = routeNode->data;
        startMatch = false;
        endMatch = false;
        startMatch = (calcDistBetWpts(&startWpt, curRoute->waypoints->head->data) < delta);
        endMatch = (calcDistBetWpts(&endWpt, curRoute->waypoints->tail->data) < delta);
        if (startMatch && endMatch) {
            if (routeList->length < 1) {
                newRouteNode = malloc(sizeof(Node));
                newRouteNode->data = curRoute;
                newRouteNode->next = NULL;
                newRouteNode->previous = NULL;
                routeList->head = newRouteNode;
                routeList->tail = newRouteNode;
                routeList->length = 1;
            } else {
                newRouteNode = malloc(sizeof(Node));
                newRouteNode->data = curRoute;
                newRouteNode->next = NULL;
                newRouteNode->previous = routeList->tail;
                routeList->tail->next = newRouteNode;
                routeList->tail = newRouteNode;
                (routeList->length)++;
            }
        }
        routeNode = routeNode->next;
    }

    if (routeList->length > 0) {
        return routeList;
    } else {
        freeList(routeList);
        return NULL;
    }
}


List* newTracksBetweenList() {
    List *newList = malloc(sizeof(List));

    newList->head = NULL;
    newList->tail = NULL;
    newList->length = 0;
    newList->deleteData = dummyDelete;
    newList->compare = compareTracks;
    newList->printData = trackToString;

    return newList;
}


List* getTracksBetween(const GPXdoc* doc, float sourceLat, float sourceLong, float destLat, float destLong, float delta) {
    List* trackList = NULL;
    Node *trackNode = NULL, *newTrackNode = NULL;
    Track *curTrack = NULL;
    Waypoint startWpt;
    Waypoint endWpt;
    TrackSegment *firstSeg;
    TrackSegment *lastSeg;
    bool startMatch = false, endMatch = false;
    if (doc == NULL) {
        return NULL;
    }
    if (doc->tracks->head == NULL) {
        return NULL;
    }

    startWpt.longitude = sourceLong;
    startWpt.latitude = sourceLat;
    endWpt.longitude = destLong;
    endWpt.latitude = destLat;

    trackList = newTracksBetweenList();

    trackNode = doc->tracks->head;
    while (trackNode != NULL) {
        curTrack = trackNode->data;
        startMatch = false;
        endMatch = false;
        firstSeg = curTrack->segments->head->data;
        lastSeg = curTrack->segments->tail->data;
        startMatch = (calcDistBetWpts(&startWpt, firstSeg->waypoints->head->data) < delta);
        endMatch = (calcDistBetWpts(&endWpt, lastSeg->waypoints->tail->data) < delta);
        if (startMatch && endMatch) {
            if (trackList->length < 1) {
                newTrackNode = malloc(sizeof(Node));
                newTrackNode->data = curTrack;
                newTrackNode->next = NULL;
                newTrackNode->previous = NULL;
                trackList->head = newTrackNode;
                trackList->tail = newTrackNode;
                trackList->length = 1;
            } else {
                newTrackNode = malloc(sizeof(Node));
                newTrackNode->data = curTrack;
                newTrackNode->next = NULL;
                newTrackNode->previous = trackList->tail;
                trackList->tail->next = newTrackNode;
                trackList->tail = newTrackNode;
                (trackList->length)++;
            }
        }
        trackNode = trackNode->next;
    }

    if (trackList->length > 0) {
        return trackList;
    } else {
        freeList(trackList);
        return NULL;
    }
}


char* gpxDataToJSON(GPXData *data) {
    char *str = malloc(3);
    /*strcpy(str, "It's a JSON String!");
    return str;*/

    if (data == NULL) {
        strncpy(str, "{}", 3); 
        return str;
    }

    strncpy(str, "{", 2);

    addJSONString(&str, "\"name\":\"");
    addJSONString(&str, data->name);
    addJSONString(&str, "\",");

    addJSONString(&str, "\"value\":\"");
    addJSONString(&str, data->value);

    addJSONString(&str, "\"}");
    return str;
}


char* gpxDataListToJSON(List *list) {
    char *str = malloc(3);
    char *temp = NULL;
    Node *cur_node = NULL;

    if (list == NULL) {
        strncpy(str, "[]", 3); 
        return str;
    }

    strncpy(str, "[", 2);

    cur_node = list->head;

    while (cur_node) {
        temp = gpxDataToJSON(cur_node->data);
        addJSONString(&str, temp);
        free(temp);
        if (cur_node->next) {
            addJSONString(&str, ",");
        }
        cur_node = cur_node->next;
    }

    addJSONString(&str, "]");

    return str;
}


char* trackToJSON(const Track *tr) {
    char *str = malloc(3);
    char temp[50];

    if (tr == NULL) {
        strncpy(str, "{}", 3); 
        return str;
    }

    strncpy(str, "{", 2);

    addJSONString(&str, "\"name\":\"");
    addJSONString(&str, tr->name);
    addJSONString(&str, "\",");

    addJSONString(&str, "\"len\":");
    sprintf(temp, "%.1f", round10(getTrackLen(tr)));
    addJSONString(&str, temp);

    addJSONString(&str, ",\"loop\":");
    if (isLoopTrack(tr, 10.0)) {
        addJSONString(&str, "true");
    } else {
        addJSONString(&str, "false");
    }

    addJSONString(&str, "}");

    return str;
}


int getSumSegPoints(Track *tr) {
    int numPoints = 0;
    Node *curNode = tr->segments->head;
    TrackSegment *curSegment = NULL;

    while (curNode) {
        curSegment = curNode->data;
        numPoints += curSegment->waypoints->length;
        curNode = curNode->next;
    }
    return numPoints;
}


char* newTrackToJSON(Track *tr) {
    char *str = malloc(3);
    char temp[50];
    int numPoints = 0;

    if (tr == NULL) {
        strncpy(str, "{}", 3); 
        return str;
    }

    strncpy(str, "{", 2);

    addJSONString(&str, "\"name\":\"");
    addJSONString(&str, tr->name);
    addJSONString(&str, "\",");

    addJSONString(&str, "\"numPoints\":");
    numPoints = getSumSegPoints(tr);
    sprintf(temp, "%d", numPoints);
    addJSONString(&str, temp);
    addJSONString(&str, ",");

    addJSONString(&str, "\"len\":");
    sprintf(temp, "%.1f", round10(getTrackLen(tr)));
    addJSONString(&str, temp);

    addJSONString(&str, ",\"loop\":");
    if (isLoopTrack(tr, 10.0)) {
        addJSONString(&str, "true");
    } else {
        addJSONString(&str, "false");
    }

    addJSONString(&str, "}");

    return str;
}

char* pointToJSON(const Waypoint *pt, int num) {
    char *str = malloc(3);
    char temp[50];

    if (pt == NULL) {
        strncpy(str, "{}", 3); 
        return str;
    }

    strncpy(str, "{", 2);

    addJSONString(&str, "\"name\":\"");
    if (pt->name != NULL) {
        addJSONString(&str, pt->name);
    } else {
        addJSONString(&str, "None");
    }

    addJSONString(&str, "\",");
    addJSONString(&str, "\"lon\":");
    sprintf(temp, "%.6f", pt->longitude);
    addJSONString(&str, temp);
    addJSONString(&str, ",");

    addJSONString(&str, "\"lat\":");
    sprintf(temp, "%.6f", pt->latitude);
    addJSONString(&str, temp);
    addJSONString(&str, ",");

    addJSONString(&str, "\"index\":");
    sprintf(temp, "%d", num);
    addJSONString(&str, temp);


    addJSONString(&str, "}");

    return str;
}


char* pointListToJSON(const List *list) {
    char *str = malloc(3);
    char *temp = NULL;
    Node *cur_node = NULL;
    int index = 0;

    if (list == NULL) {
        strncpy(str, "[]", 3); 
        return str;
    }

    strncpy(str, "[", 2);

    cur_node = list->head;

    while (cur_node) {
        temp = pointToJSON(cur_node->data, index);
        addJSONString(&str, temp);
        free(temp);
        if (cur_node->next) {
            addJSONString(&str, ",");
        }
        cur_node = cur_node->next;
        index++;
    }

    addJSONString(&str, "]");

    return str;
}


char* routeToJSON(const Route *rt) {
    char *str = malloc(3);
    char temp[50];

    if (rt == NULL) {
        strncpy(str, "{}", 3); 
        return str;
    }

    strncpy(str, "{", 2);

    addJSONString(&str, "\"name\":\"");
    if (rt->name != NULL) {
        addJSONString(&str, rt->name);
    } else {
        addJSONString(&str, "None");
    }

    addJSONString(&str, "\",\"");
    addJSONString(&str, "numPoints\":");
    sprintf(temp, "%d", rt->waypoints->length);
    addJSONString(&str, temp);
    addJSONString(&str, ",");

    addJSONString(&str, "\"len\":");
    sprintf(temp, "%.1f", round10(getRouteLen(rt)));
    addJSONString(&str, temp);

    addJSONString(&str, ",\"loop\":");
    if (isLoopRoute(rt, 10.0)) {
        addJSONString(&str, "true");
    } else {
        addJSONString(&str, "false");
    }

    addJSONString(&str, "}");

    return str;
}


char* routeListToJSON(const List *list) {
    char *str = malloc(3);
    char *temp = NULL;
    Node *cur_node = NULL;

    if (list == NULL) {
        strncpy(str, "[]", 3); 
        return str;
    }

    strncpy(str, "[", 2);

    cur_node = list->head;

    while (cur_node) {
        temp = routeToJSON(cur_node->data);
        addJSONString(&str, temp);
        free(temp);
        if (cur_node->next) {
            addJSONString(&str, ",");
        }
        cur_node = cur_node->next;
    }

    addJSONString(&str, "]");

    return str;
}


char* trackListToJSON(const List *list) {
    char *str = malloc(3);
    char *temp = NULL;
    Node *cur_node = NULL;

    if (list == NULL) {
        strncpy(str, "[]", 3); 
        return str;
    }

    strncpy(str, "[", 2);

    cur_node = list->head;

    while (cur_node) {
        temp = trackToJSON(cur_node->data);
        addJSONString(&str, temp);
        free(temp);
        if (cur_node->next) {
            addJSONString(&str, ",");
        }
        cur_node = cur_node->next;
    }

    addJSONString(&str, "]");

    return str;
}

char* newTrackListToJSON(const List *list) {
    char *str = malloc(3);
    char *temp = NULL;
    Node *cur_node = NULL;

    if (list == NULL) {
        strncpy(str, "[]", 3); 
        return str;
    }

    strncpy(str, "[", 2);

    cur_node = list->head;

    while (cur_node) {
        temp = newTrackToJSON(cur_node->data);
        addJSONString(&str, temp);
        free(temp);
        if (cur_node->next) {
            addJSONString(&str, ",");
        }
        cur_node = cur_node->next;
    }

    addJSONString(&str, "]");

    return str;
}


char* GPXtoJSON(const GPXdoc* gpx) {
    char *str = malloc(3);
    char temp[50];

    if (gpx == NULL) {
        strncpy(str, "{}", 3); 
        return str;
    }

    strncpy(str, "{", 2);

    addJSONString(&str, "\"version\":");
    sprintf(temp, "%.1f", gpx->version);
    addJSONString(&str, temp);
    addJSONString(&str, ",");

    addJSONString(&str, "\"creator\":\"");
    addJSONString(&str, gpx->creator);
    addJSONString(&str, "\",");

    addJSONString(&str, "\"numWaypoints\":");
    sprintf(temp, "%d", getNumWaypoints(gpx));
    addJSONString(&str, temp);
    addJSONString(&str, ",");

    addJSONString(&str, "\"numRoutes\":");
    sprintf(temp, "%d", getNumRoutes(gpx));
    addJSONString(&str, temp);
    addJSONString(&str, ",");

    addJSONString(&str, "\"numTracks\":");
    sprintf(temp, "%d", getNumTracks(gpx));
    addJSONString(&str, temp);

    addJSONString(&str, "}");

    return str;
}



/*
 *  START OF A2 BONUS FUNCTIONS
 *
 */


void addWaypoint(Route *rt, Waypoint *pt) {
    Node *new_node = makeNullListNode();
    new_node->data = pt;

    if ((rt == NULL) || (pt == NULL)) {
        return;
    }
    
    if ((rt->waypoints->head) == NULL) {
        rt->waypoints->head = new_node;
        rt->waypoints->tail = new_node;
    } else {
        new_node->previous = rt->waypoints->tail;
        rt->waypoints->tail->next = new_node;
        rt->waypoints->tail = new_node;
    }
    return;
}


void addRoute(GPXdoc* doc, Route* rt) {
    Node *new_node = makeNullListNode();
    new_node->data = rt;

    if ((doc == NULL) || (rt == NULL)) {
        return;
    }
    
    if ((doc->routes->head) == NULL) {
        doc->routes->head = new_node;
        doc->routes->tail = new_node;
    } else {
        new_node->previous = doc->routes->tail;
        doc->routes->tail->next = new_node;
        doc->routes->tail = new_node;
    }
    return;
}


GPXdoc* JSONtoGPX(const char* gpxString) {
    GPXdoc *gpx = NULL;
    char *str = NULL;
    char *temp = NULL;
    int i = 0;
    if (gpxString == NULL) {
        return NULL;
    }

    gpx = malloc(sizeof(GPXdoc));

    gpx->namespace[0] = '\0';
    gpx->version = 0.0;
    gpx->creator = NULL;
    gpx->waypoints = makeWptList();
    gpx->routes = makeRteList();
    gpx->tracks = makeTrkList();

    str = strstr(gpxString, "\"version\"");
    if (str != NULL) {
        str = strstr(str, ":");
    } else {
        gpx->version = 0.0;
    }
    if (str != NULL) {
        str++;
        gpx->version = strtof(str, &str);
    }

    str = strstr(gpxString, "\"creator\"");
    if (str != NULL) {
        str = strstr(str, ":");
        str = strstr(str, "\"");
    } else {
        gpx->creator = malloc(1);
        gpx->creator[0] = '\0';
    }
    if (str != NULL) {
        str++;
        i = 0;
        temp = malloc(1);
        temp[0] = '\0';
        while (str[0] != '\"') {
            temp = realloc(temp, i+2);
            temp[i] = str[0];
            i++;
            str++;
        }
        gpx->creator = temp;
    }

    return gpx;
}


Waypoint* JSONtoWaypoint(const char* gpxString) {
    Waypoint *wpt = NULL;
    char *str = NULL;
    if (gpxString == NULL) {
        return NULL;
    }

    wpt = malloc(sizeof(Waypoint));

    wpt->name = malloc(1);
    wpt->name[0] = '\0';

    wpt->otherData = makeGpxDataList();

    str = strstr(gpxString, "\"lat\"");
    if (str != NULL) {
        str = strstr(str, ":");
    } else {
        wpt->latitude = 0.0;
    }
    if (str != NULL) {
        str++;
        wpt->latitude = strtof(str, &str);
    }

    str = strstr(gpxString, "\"lon\"");
    if (str != NULL) {
        str = strstr(str, ":");
    } else {
        wpt->longitude = 0.0;
    }
    if (str != NULL) {
        str++;
        wpt->longitude = strtof(str, &str);
    }

    return wpt;
}


Route* JSONtoRoute(const char* gpxString) {
    Route *rt = NULL;
    char *str = NULL;
    char *temp = NULL;
    int i = 0;
    if (gpxString == NULL) {
        return NULL;
    }

    rt = malloc(sizeof(Route));

    rt->waypoints = makeWptList();
    rt->otherData = makeGpxDataList();

    str = strstr(gpxString, "\"name\"");
    if (str != NULL) {
        str = strstr(str, ":");
        str = strstr(str, "\"");
    } else {
        rt->name = malloc(1);
        rt->name[0] = '\0';
    }
    if (str != NULL) {
        str++;
        i = 0;
        temp = malloc(1);
        temp[0] = '\0';
        while (str[0] != '\"') {
            temp = realloc(temp, i+2);
            temp[i] = str[0];
            i++;
            str++;
        }
        rt->name = temp;
    }

    return rt;
}


char* CreateGPXFromServer(char* filename, char* dataJSON) {
    if (filename == NULL || dataJSON == NULL) {
        return "invalid string inputs";
    }
    char *temp = malloc(1000);
    
    
    GPXdoc *doc = JSONtoGPX(dataJSON);
    if (writeGPXdoc(doc, filename)) {
    strcpy(temp, "Created file: ");
    strcat(temp, filename);
    deleteGPXdoc(doc);
    return temp;
    } else {
        deleteGPXdoc(doc);
        return "File was not created";
    }
}


char* GetFileCharacters(char* filename, char* schema) {
    if (filename == NULL) {
        return "invalid string inputs";
    }
    char *result;

    GPXdoc *doc = createValidGPXdoc(filename, schema);
    if (doc == NULL) return "{\"version\":69.420,\"creator\":\"None\",\"numWaypoints\":0,\"numRoutes\":0,\"numTracks\":1}";
    result = GPXtoJSON(doc);
    deleteGPXdoc(doc);
    return result;
}


char* GetRouteData(char* filename, char* schema) {
    if (filename == NULL) {
        return "invalid string inputs";
    }
    char *result;

    GPXdoc *doc = createValidGPXdoc(filename, schema);
    if (doc == NULL) return "";
    result = routeListToJSON(doc->routes);
    deleteGPXdoc(doc);
    return result;
}


char* GetPointData(char* filename, char* schema, int num) {
    if (filename == NULL) {
        return "invalid string inputs";
    }
    int i;
    char *result;
    Node* cur_node = NULL;
    Route *rt = NULL;

    GPXdoc *doc = createValidGPXdoc(filename, schema);
    if (doc == NULL) return "";
    cur_node = doc->routes->head;
    for (i = 0; i < num; i++) {
        if (cur_node == NULL) {
            return false;
        }
        cur_node = cur_node->next;
    }
    rt = cur_node->data;
    if (rt == NULL) {
        return "";
    }

    result = pointListToJSON(rt->waypoints);
    deleteGPXdoc(doc);
    return result;
}


char* GetTrackData(char* filename, char* schema) {
    if (filename == NULL) {
        return "invalid string inputs";
    }
    char *result;

    GPXdoc *doc = createValidGPXdoc(filename, schema);
    if (doc == NULL) {
        return "";
    }
    result = newTrackListToJSON(doc->tracks);
    deleteGPXdoc(doc);
    return result;
}


bool ChangeRouteName(char* filename, char* newName, int num, char* schema) {
    if ((filename == NULL) || (newName == NULL) || (num == 0) || (schema == NULL)) {
        return false;
    }
    int i;
    Node* cur_node = NULL;
    Route *rt = NULL;

    GPXdoc *doc = createValidGPXdoc(filename, schema);
    cur_node = doc->routes->head;
    for (i = 1; i < num; i++) {
        if (cur_node == NULL) {
            return false;
        }
        cur_node = cur_node->next;
    }

    rt = cur_node->data;
    if (rt == NULL) {
        return false;
    }
    rt->name = realloc(rt->name, strlen(newName) + 1);
    strcpy(rt->name, newName);

    if (writeGPXdoc(doc, filename)){
        deleteGPXdoc(doc);
        return true;
    } else {
        deleteGPXdoc(doc);
        return false;
    }
}


bool ChangeTrackName(char* filename, char* newName, int num, char* schema) {
    if ((filename == NULL) || (newName == NULL) || (num == 0) || (schema == NULL)) {
        return false;
    }
    int i;
    Node *cur_node = NULL;
    Track *tr = NULL;

    GPXdoc *doc = createValidGPXdoc(filename, schema);
    cur_node = doc->tracks->head;
    for (i = 1; i < num; i++) {
        if (cur_node == NULL) {
            return false;
        }
        cur_node = cur_node->next;
    }

    tr = cur_node->data;
    if (tr == NULL) {
        return false;
    }

    tr->name = realloc(tr->name, strlen(newName) + 1);
    strcpy(tr->name, newName);

    if (writeGPXdoc(doc, filename)){
        deleteGPXdoc(doc);
        return true;
    } else {
        deleteGPXdoc(doc);
        return false;
    }
}


bool ValidateFile(char* filename, char* schema) {
    if (filename == NULL) {
        return "invalid string inputs";
    }
    GPXdoc *doc = createValidGPXdoc(filename, schema);
    if (doc == NULL) {
        return false;
    } else {
        return true;
    }
}


char* GetGPXDataFromRoute(char* filename, int num, char* schema) {
    if ((filename == NULL) || (num == 0) || (schema == NULL)) {
        return "unable to open file route";
    }
    int i;
    Node* cur_node = NULL;
    Route *rt = NULL;
    List *gpxList = NULL;
    char* str = NULL;

    GPXdoc *doc = createValidGPXdoc(filename, schema);
    cur_node = doc->routes->head;
    for (i = 1; i < num; i++) {
        if (cur_node == NULL) {
            return "No data available route 1";
        }
        cur_node = cur_node->next;
    }

    rt = cur_node->data;
    if (rt == NULL) {
        return "No data available 2";
    }
    gpxList = rt->otherData;
    str = gpxDataListToJSON(gpxList);

    deleteGPXdoc(doc);

    if (str != NULL){
        return str;
    } else {
        return "No data available route 3";
    } 
}


char* GetGPXDataFromTrack(char* filename, int num, char* schema) {
    if ((filename == NULL) || (num == 0) || (schema == NULL)) {
        return "unable to open file";
    }
    int i;
    Node* cur_node = NULL;
    Track *tr = NULL;
    List *gpxList = NULL;
    char* str = NULL;

    GPXdoc *doc = createValidGPXdoc(filename, schema);
    cur_node = doc->tracks->head;
    for (i = 1; i < num; i++) {
        if (cur_node == NULL) {
            return "No data available track 1";
        }
        cur_node = cur_node->next;
    }

    tr = cur_node->data;
    if (tr == NULL) {
        return "No data available track 2";
    }
    gpxList = tr->otherData;
    str = gpxDataListToJSON(gpxList);

    deleteGPXdoc(doc);

    if (str != NULL){
        return str;
    } else {
        return "No data available track 3";
    } 
}

char* GetRoutePathBetween (char *filename, char *schema, float startLat, float startLon, float endLat, float endLon, float accuracy) {
    if (filename == NULL) {
        return "invalid file inputs";
    }
    //return "[{\"A\":\"whoops\"},{\"yeah\":\"butno\"}]";
    char *result;
    List *list = NULL;

    GPXdoc *doc = createValidGPXdoc(filename, schema);
    list = getRoutesBetween(doc, startLat, startLon, endLat, endLon, accuracy);

    if (doc == NULL) {
        return "[{\"a\":\"b\"}]";
    }
    result = routeListToJSON(list);
    deleteGPXdoc(doc);
    return result;
}

char* GetTrackPathBetween (char *filename, char *schema, float startLat, float startLon, float endLat, float endLon, float accuracy) {
    if (filename == NULL) {
        return "invalid file inputs";
    }
    //return "[{\"A\":\"whoops\"},{\"yeah\":\"butno\"}]";
    char *result;
    List *list = NULL;

    GPXdoc *doc = createValidGPXdoc(filename, schema);
    list = getTracksBetween(doc, startLat, startLon, endLat, endLon, accuracy);

    if (doc == NULL) {
        return "[{\"a\":\"b\"}]";
    }
    result = newTrackListToJSON(list);
    deleteGPXdoc(doc);
    return result;
}

bool AddRouteToFile (char *filename, char *schema, char *routeJSON) {
    if ((filename == NULL) || (routeJSON == NULL)) {
        return false;
    }
    Route *rt = NULL;
    List *rtList = NULL;
    Node *newNode = makeNullListNode();

    GPXdoc *doc = createValidGPXdoc(filename, schema);
    rtList = doc->routes;
    rt = JSONtoRoute(routeJSON);
    newNode->data = rt;
    
    if(rtList->tail != NULL) {
        rtList->tail->next = newNode;
        newNode->previous = rtList->tail;
        rtList->tail = newNode;
        rtList->length++;
    } else {
        rtList->head = newNode;
        rtList->tail = newNode;
        rtList->length++;
    }

    if (writeGPXdoc(doc, filename)){
        deleteGPXdoc(doc);
        return true;
    } else {
        deleteGPXdoc(doc);
        return false;
    }
}

bool AddWaypointToFile (char *filename, char *schema, char *wptJSON) {
    if ((filename == NULL) || (wptJSON == NULL)) {
        return false;
    }
    Waypoint *wpt = NULL;
    List *wptList = NULL;
    Node *newNode = makeNullListNode();

    GPXdoc *doc = createGPXdoc(filename);
    wptList = ((Route*)doc->routes->tail->data)->waypoints;
    wpt = JSONtoWaypoint(wptJSON);
    newNode->data = wpt;
    
    if(wptList->tail != NULL) {
        wptList->tail->next = newNode;
        newNode->previous = wptList->tail;
        wptList->tail = newNode;
        wptList->length++;
    } else {
        wptList->head = newNode;
        wptList->tail = newNode;
        wptList->length++;
    }

    if (writeGPXdoc(doc, filename)){
        deleteGPXdoc(doc);
        return true;
    } else {
        deleteGPXdoc(doc);
        return false;
    }
}