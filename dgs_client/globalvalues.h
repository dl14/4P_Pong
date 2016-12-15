#ifndef GLOBALVALUES_H
#define GLOBALVALUES_H

// Version 1.1 as of 11/22/16 @ 5:30pm

#include <QString>
class ShapeGlobals {
public:
    // Basic formatting of a message & payload
    // Message must be a JSON object and have the required Key:Value pairs
    // If the JSON parser encounters any of the following errors, then it will close the connection:
    static QString Rule0_1() {return(QString("Rule 0.1: Got a JSON Object, but did not have the minimal required values.  Closing connection."));};
    static QString Rule0_2(QString errorMsg) {return(QString("Rule 0.2: Got a JSON Parser error that is not fixable.  Closing connection. :").arg(errorMsg));};
    static QString Rule0_3() {return(QString("Rule 0.3: Did not receive a response within a reasonable waiting time. Closing connection."));};

    // Version create/delete/update related rules
    static QString Rule1_1() {return(QString("Rule 1.1: If the object already exists, then the request to create it will fail"));};
    static QString Rule1_2() {return(QString("Rule 1.2: If the object doesn't exist, then any request for it other than create will fail"));};
    static QString Rule1_3() {return(QString("Rule 1.3: If the object exists, then the incoming request must either be a delete or its version number must be existing_version+1"));};
    static QString Rule1_4() {return(QString("Rule 1.4: May not create a UserPayload with a UserName that is already in an existing UserPayload"));};
    static QString Rule1_5() {return(QString("Rule 1.5: UserPayload already associated with an active client connection."));};
    static QString Rule1_6() {return(QString("Rule 1.6: An update to the FieldPayload must have a FieldBoundary that completely contains the FieldBoundary of the existing version."));};
    static QString Rule1_7() {return(QString("Rule 1.7: If a FieldPayload object exists, must not create a new one."));};
    static QString Rule1_8() {return(QString("Rule 1.8: The ObjOwnerUuid in the Payload must be the UUID of a user that exists in the repository."));};

    // Permission related rules
    static QString Rule2_FieldPayload() {return(QString("Rule 2.FieldPayload: Do not have permission to operate on a FieldPayload"));};
    static QString Rule2_Default(QString payloadType) {return(QString("Rule 2.Default: Do not have permission to operate on this payload of type %1").arg(payloadType));};
    static QString Rule2_ShapePayload() {return(QString("Rule 2.ShapePayload: Do not have permission to operate on this particular ShapePayload"));};
    static QString Rule2_UserPayload() {return(QString("Rule 2.UserPayload: Do not have permission to operate on this particular UserPayload"));};

    // Shape intersection related rules
    static QString Rule3_1_1() {return(QString("Rule 3.1.1: Request to create a new shape that isn't contained within the field"));};
    static QString Rule3_1_2() {return(QString("Rule 3.1.2: Request to update an existing shape results in it not contained within the field"));};
    static QString Rule3_2_1() {return(QString("Rule 3.2.1: Request to create a new shape at an intersecting position"));};
    static QString Rule3_2_2_Rectangle() {return(QString("Rule 3.2.2.Rectangle: If a request to update a shape of type Rectangle will cause it to intersect, then it is rejected"));};
    static QString Rule3_2_2_Default() {return(QString("Rule 3.2.2.Default: If a request to update a shape of type <default> will cause it to intersect, then it is rejected"));};

    // Initial connection related rules
    static QString Rule4_1() {return(QString("Rule 4.1: Initial request to update user object be the first payload and succeed, Closing connection.; "));};
    static QString Rule4_2() {return(QString("Rule 4.2: Received a message from the client before initialization was complete.  Closing connection."));};
    static QString Rule4_3() {return(QString("Rule 4.3: The first message from the client must be the client's user object.  Closing connection."));};
    static QString Rule4_4() {return(QString("Rule 4.4: Must wait for a response to the initial user object before sending anything else.  Closing connection."));};
    static QString Rule4_5() {return(QString("Rule 4.5: This server requires the first client to be the Game Play Client with the GameMaster User.  Closing connection."));};

    // Indication of whether a message was processed or rejected
    static QString MessageProcessed() {return(QString("Processed"));};
    static QString MessageRejected() {return(QString("Rejected"));};

    // Meaning of object versions.  All updates are > 1
    static int CreateObjectVersion() {return 1;};
    static int DeleteObjectVersion() {return 0;};
    static int InvalidObjectVersion() {return -1;};

    // Meaning of authorization levels
    static int AllPermissions() {return 0;};
    static int GMPermissions() {return 1;};
    static int DefaultUserPermissions() {return 2;};
    static int SpecialUserPermissions() {return 3;};
};

#endif // GLOBALVALUES_H
