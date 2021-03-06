/*
 * Note: this file originally auto-generated by mib2c using
 *        $
 */

#include <net-snmp/net-snmp-config.h>
#include <net-snmp/net-snmp-includes.h>
#include <net-snmp/agent/net-snmp-agent-includes.h>
#include "notificationObjs.h"
#include "sysAlarms.h"

/*
 * notificationObjs_variables_oid:
 *   this is the top level oid that we want to register under.  This
 *   is essentially a prefix, with the suffix appearing in the
 *   variable below.
 */

oid             notificationObjs_variables_oid[] =
    { 1, 3, 6, 1, 4, 1, 8072, 9999, 9999, 1, 11 };

/*
 * variable4 notificationObjs_variables:
 *   this variable defines function callbacks and type return information 
 *   for the notificationObjs mib section 
 */

struct variable4 notificationObjs_variables[] = {
    /*
     * magic number        , variable type , ro/rw , callback fn  , L, oidsuffix 
     */
#define ALARMCOUNTER		1
    {ALARMCOUNTER, ASN_INTEGER, NETSNMP_OLDAPI_RWRITE,
     var_notificationObjs, 1, {3}},

};

/*
 * (L = length of the oidsuffix) 
 */


/** Initializes the notificationObjs module */
void
init_notificationObjs(void)
{

    DEBUGMSGTL(("notificationObjs", "Initializing\n"));

    /*
     * register ourselves with the agent to handle our mib tree 
     */
    REGISTER_MIB("notificationObjs", notificationObjs_variables, variable4,
                 notificationObjs_variables_oid);

    /*
     * place any other initialization junk you need here 
     */
     // 注册周期读取告警的函数
     register_read_alarmdata();
     
}

/*
 * var_notificationObjs():
 *   This function is called every time the agent gets a request for
 *   a scalar variable that might be found within your mib section
 *   registered above.  It is up to you to do the right thing and
 *   return the correct value.
 *     You should also correct the value of "var_len" if necessary.
 *
 *   Please see the documentation for more information about writing
 *   module extensions, and check out the examples in the examples
 *   and mibII directories.
 */
unsigned char  *
var_notificationObjs(struct variable *vp,
                     oid * name,
                     size_t *length,
                     int exact,
                     size_t *var_len, WriteMethod ** write_method)
{
	static long VAR;


    if (header_generic(vp, name, length, exact, var_len, write_method)
        == MATCH_FAILED)
        return NULL;

    /*
     * this is where we do the value assignments for the mib results.
     */
    switch (vp->magic) {
    case ALARMCOUNTER:
        *write_method = write_alarmCounter;
 	    *var_len = sizeof(long); 		
        snmp_get_data(SHM_ALARM,ALARM_COUNTER,sizeof(long),&VAR); 
        return (u_char *) &VAR;
    default:
        ERROR_MSG("");
    }
    return NULL;
}



int
write_alarmCounter(int action,
                   u_char * var_val,
                   u_char var_val_type,
                   size_t var_val_len,
                   u_char * statP, oid * name, size_t name_len)
{
    long            value;

    switch (action) {
    case RESERVE1:
        if (var_val_type != ASN_INTEGER) {
            fprintf(stderr, "write to notificationObjs not ASN_INTEGER\n");
            return SNMP_ERR_WRONGTYPE;
        }
        if (var_val_len > sizeof(long)) {
            fprintf(stderr, "write to notificationObjs: bad length\n");
            return SNMP_ERR_WRONGLENGTH;
        }
        break;

    case RESERVE2:
        //size = var_val_len;
        value = *(long *) var_val;

        break;

    case FREE:
        /*
         * Release any resources that have been allocated 
         */
        break;

    case ACTION:
        /*
         * The variable has been stored in 'value' for you to use,
         * and you have just been asked to do something with it.
         * Note that anything done here must be reversable in the UNDO case
         */
         if( snmp_set_data(SHM_ALARM,ALARM_COUNTER,var_val_len,&value) <= 0)  
			return SNMP_ERR_WRONGVALUE;
        break;

    case UNDO:
        /*
         * Back out any changes made in the ACTION case 
         */
        break;

    case COMMIT:
        /*
         * Things are working well, so it's now safe to make the change
         * permanently.  Make sure that anything done here can't fail!
         */
        break;
    }
    return SNMP_ERR_NOERROR;
}
