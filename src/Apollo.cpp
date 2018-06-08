
#include <iostream>
#include <cstdint>
#include <string>

#include "Apollo.h"
//
#include "caliper/cali.h"
#include "caliper/Annotation.h"
//
#include "sos.h"
#include "sos_types.h"

SOS_runtime *sos;
SOS_pub     *pub;


void 
handleFeedback(int msg_type, int msg_size, void *data)
{

    switch (msg_type) {
        //
        case SOS_FEEDBACK_TYPE_QUERY:
            apollo_log(1, "Query results received."
                    "  (msg_size == %d)\n", msg_size);
            break;
        //
        case SOS_FEEDBACK_TYPE_PAYLOAD:
            apollo_log(1, "Trigger payload received."
                    "  (msg_size == %d, data == %s\n",
                    msg_size, (char *) data);
            break;
    }


    return;
}

Apollo::Apollo()
{
    ynConnectedToSOS = false;

    sos = NULL;
    pub = NULL;

    SOS_init(&sos, SOS_ROLE_CLIENT,
            SOS_RECEIVES_DIRECT_MESSAGES, handleFeedback);

    if (sos == NULL) {
        fprintf(stderr, "APOLLO: Unable to communicate with the SOS daemon.\n");
        return;
    }

    SOS_pub_init(sos, &pub, (char *)"APOLLO", SOS_NATURE_SUPPORT_EXEC);

    if (pub == NULL) {
        fprintf(stderr, "APOLLO: Unable to create publication handle.\n");
        SOS_finalize(sos);
        sos = NULL;
        return;
    }

    ynConnectedToSOS = true; 

    apollo_log(0, "Initialized.\n");

    return;
}



Apollo::~Apollo()
{
    if (sos != NULL) {
        SOS_finalize(sos);
        sos = NULL;
    }
}

Apollo::Region::Region(Apollo *apollo_ptr, const char *regionName)
{
    apollo = apollo_ptr;
    name = strdup(regionName);

    cali_obj = NULL;
    cali_iter_obj = NULL;
    ynInsideMarkedRegion = false;

    return;
}


void
Apollo::Region::begin(void) {
    if (ynInsideMarkedRegion == true) {
        // Free up the old region and make a new one,
        // to comply with Caliper "constructor == start"
        // conventions.
        //
        cali_obj->end();
        delete cali_obj;
        
    }
    cali_obj = new cali::Loop(name);
    ynInsideMarkedRegion = true;
    return;
}

void
Apollo::Region::iteration_start(int i) {
    if (ynInsideMarkedRegion == true) {
        if (cali_iter_obj != NULL) {
            delete cali_iter_obj;
        }
        cali_iter_obj = new cali::Loop::Iteration(
                cali_obj->iteration(static_cast<int>(i))  );
    } else {
        // Do nothing.  (There is nothing to iterate on.)
    } 

    return;  
}

void
Apollo::Region::iteration_stop(void) {
    if (cali_iter_obj != NULL) {
        delete cali_iter_obj;
        cali_iter_obj = NULL;
    }
    return;
}


void
Apollo::Region::end(void) {
    ynInsideMarkedRegion = false;

    if (cali_iter_obj != NULL) {
        delete cali_iter_obj;
        cali_iter_obj = NULL;
    }

    cali_obj->end();
    delete cali_obj;
    cali_obj = NULL;

    return;
}


void
Apollo::Region::named_int(const char *name, int value) {
    cali_set_int_byname(name, value); 
    return;
}

void
Apollo::Region::ind_feature(const char *name, int dataType, void *value) {
    //TODO
    return;
}

void
Apollo::Region::dep_feature(const char *name, int dataType, void *value) {
    //TODO
    return;
}

Apollo::Region::~Region()
{
    free(name);
    return;
}


bool Apollo::isOnline()
{
    return ynConnectedToSOS;
}



