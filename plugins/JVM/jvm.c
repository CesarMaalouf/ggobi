/**
  This file provides the basic C-level methods for a Java
  plugin for GGobi. What this means is that we can load the
  Java Virtual Machine (JVM) inside the GGobi process and
  access Java classes. In practice, this will mean that we
  can implement plugins (both regular and input plugins)
  using Java classes. 
  Additionally, the file ggobiAccess.c provides native methods
  for Java classes that allow those Java classes to dynamically
  access the GGobi instances and their data structures.
  When combined with the Gtk bindings for Java (http://java-gnome.sourceforge.net/)
  one can develop plugins that extend both the functionality and appearance
  of GGobi via Java code rather than lower-level C code.
 */

#include "jvm.h"
#include "GGobiAPI.h"


/**
 The default Java context in which to execute calls.
 */
static JNIEnv *std_env = NULL;

/**
  The JVM. Not necessary to keep it here as we can get it
  at any time via the JNI method GetJavaVM().
 */
static JavaVM *jvm;

/**
   the identifier for the classpath option when starting the JVM.
 */
#define CLASSPATH_OPT "-Djava.class.path="


gboolean initJVM(void);
jobject  runPlugin(const char * const klass, JNIEnv *env);
char *   getInputDescription(JavaInputPluginData *rt, JNIEnv *env);
jboolean getDims(int *nrow, int *ncol, JavaRunTimeData *rt, JNIEnv *env);

/**
  This is the initial entry point for the plugin that starts the
  virtual machine.
  @see initJVM()
 */
gboolean loadJVM(gboolean initializing, GGobiPluginInfo *pluginInfo)
{
    return(initJVM());
} 

/**
  This actually starts the Virtual Machine.
 
 */
gboolean initJVM()
{
  jboolean status = JNI_FALSE;
  JavaVMInitArgs vm2_args;
  JavaVMInitArgs *vm_args = &vm2_args;
  JavaVMOption options[1];
  char *ptr;
  jint res;

  /* This is the default classpath. Need to modify this to be dynamically 
     specified by the plugin information.
   */
  char *classpath="plugins/JVM";

  /* Prepare the arguments used to initialize the JVM. Note we only get one chance. */
    vm_args = (JavaVMInitArgs *) &vm2_args;
    vm_args->version = JNI_VERSION_1_2;

    /* Set the classpath option by allocating the space for the string and filling it in. */
    options[0].optionString = ptr = (char*) calloc(strlen(CLASSPATH_OPT) + strlen(classpath) + 1, sizeof(char));
    strcpy(ptr, CLASSPATH_OPT);
    strcat(ptr, classpath);

    /* Now fix up the arguments and start the JVM. */
    vm_args->options = options;
    vm_args->nOptions = sizeof(options)/sizeof(options[0]);
    vm_args->ignoreUnrecognized = JNI_FALSE;
    if(JNI_GetDefaultJavaVMInitArgs(vm_args) != 0) {
	return(JNI_FALSE);
    }

    /* Start the JVM. */
    res = JNI_CreateJavaVM(&jvm, (void**) &std_env, (void*) vm_args);

    if(res < 0 || std_env == NULL) {
	fprintf(stderr, "Can't create JVM\n");
        return(JNI_FALSE);
    }

    status = JNI_TRUE;
    return(status);
}

/*******************************************************************************
  
                     Generic support for Java Input Plugins.

 There are basically two methods: get the description and read the data.

 *******************************************************************************/


/**
 This method pulls the information to create the datad in GGobi from the Java
 plugin instance. 
 It asks for the dimensions of the dataset using the getNumRecords() and getNumVariables()
 methods. It uses this to initialize the datad via the C routine datad_create.
 Then it queries the object for the variable names and assigns each one to the
 datad via  GGobi_setVariableName() method.
 Then it loops over the records and asks for an array of the values for each one.
 This is done via getRecord() and it copies the contents of the array to the 
 corresponding row in the datad values array.
 Finally, it gives the GGobi instance a shove to initialize the dataset
 and popup a new plot.
 */
gboolean 
JavaReadInput(InputDescription *desc, ggobid *gg, GGobiInputPluginInfo *plugin)
{
    datad *gdata;
    JavaRunTimeData *rt = ((JavaInputPluginData *) plugin->data)->runTime;
    int i, j;
    int nrow, ncol;

    jmethodID mid;
    jobjectArray jnames;
    jstring jname;
    JNIEnv *env = std_env;

/* Need to figure out who is actually calling this and if it supplies the init. */
gboolean init = true;

      /* Ask the Java instance for the dimensions of the new dataset. */
    if(getDims(&nrow, &ncol, rt, env) == JNI_FALSE)
	return(false);

      /* Create the new datad in the GGobi instance and get ready to populate it.*/
    gdata = datad_create(nrow, ncol, gg);

    gdata->name = g_strdup(desc->fileName);

    for(j = 0; j < ncol; j++) {
	const char *tmp;
        jboolean isCopy;
        mid = JVMENV GetMethodID(env, rt->klass, "getVariableNames", "()[Ljava/lang/String;");
        jnames = JVMENV CallObjectMethod(env, rt->instance, mid);
        jname = JVMENV GetObjectArrayElement(env, jnames, j);
        tmp =  JVMENV GetStringUTFChars(env, jname, &isCopy);
	GGOBI(setVariableName)(j, g_strdup(tmp), false, gdata, gg);
	JVMENV ReleaseStringUTFChars(env, jname, tmp);
    }

    mid = JVMENV GetMethodID(env, rt->klass, "getRecord", "(I)[D");
    for(i = 0 ; i < nrow; i++) {
        jdouble *els;
        jboolean isCopy;
        jdoubleArray obj = JVMENV CallObjectMethod(env, rt->instance, mid, (jint) i);
        els = JVMENV GetDoubleArrayElements(env, obj, &isCopy);
        for(j = 0; j < ncol; j++)
	    gdata->raw.vals[i][j] = els[j];
        JVMENV ReleaseDoubleArrayElements(env, obj, els, JNI_ABORT);
    }

    start_ggobi(gg, true, init);

    return(true);
}


/**
  This is the routine that gets a description of the dataset that is
  to be read in by the Input Plugin for the given file and data mode.
  It starts by creating the Java object for this plugin instance and
  calling its constructor with two arguments: the file name and the 
  data mode name.
 */
InputDescription *
JavaGetInputDescription(const char * const fileName, const char * const modeName,
                         ggobid *gg, GGobiInputPluginInfo *info)
{
    JavaInputPluginData *data = (JavaInputPluginData *) info->data;
    InputDescription *desc;
    JNIEnv *env = std_env;

      /* */
    if(data->runTime == NULL) {
        jclass klass;
        jmethodID cid;
        jstring jfn = NULL, jmode = NULL;

	data->runTime = g_malloc(sizeof(JavaRunTimeData));
        memset(data->runTime, '\0',sizeof(JavaRunTimeData));

        klass = JVMENV FindClass(env, data->className);
        cid = JVMENV GetMethodID(env, klass, "<init>", "(Ljava/lang/String;Ljava/lang/String;)V");
        if(fileName)
	    jfn = JVMENV NewStringUTF(env, fileName);
        if(modeName)
	    jmode = JVMENV NewStringUTF(env, modeName);
        data->runTime->instance = JVMENV NewObject(env, klass, cid, jfn, jmode);
	data->runTime->klass = klass;
    }

    desc = (InputDescription *) g_malloc(sizeof(InputDescription));
    desc->fileName = getInputDescription(data, env);
#ifdef JAVA_DEBUG
    fprintf(stderr, "Input source fileName: %s\n", desc->fileName);fflush(stderr);
#endif
    desc->mode = unknown_data;
    desc->desc_read_input = JavaReadInput;
    return(desc);
}


/**
  This is the routine that calls the Java method getSourceDescription()
  for the plugin object and gets a brief description of the plugin data
  source. The idea is that this description can be used in the 
 */
char *
getInputDescription(JavaInputPluginData *data, JNIEnv *env)
{

    jmethodID mid;
    jobject val;
    char *ptr;
    jboolean isCopy;
    jstring jstr;
    const char *tmp;

    mid = JVMENV GetMethodID(env, data->runTime->klass, "getSourceDescription", "(Z)Ljava/lang/String;");
    jstr = (jstring) JVMENV CallObjectMethod(env, data->runTime->instance, mid);
    
    tmp =  JVMENV GetStringUTFChars(env, jstr, &isCopy);
    
    ptr = g_strdup(tmp);
    JVMENV ReleaseStringUTFChars(env, jstr, tmp);
    return(ptr);
}


jboolean
getDims(int *nrow, int *ncol, JavaRunTimeData *rt, JNIEnv *env)
{
    jmethodID mid;

    mid = JVMENV GetMethodID(env, rt->klass, "getNumRecords", "()I");
    *nrow = JVMENV CallIntMethod(env, rt->instance, mid);

    mid = JVMENV GetMethodID(env, rt->klass, "getNumVariables", "()I");
    *ncol = JVMENV CallIntMethod(env, rt->instance, mid);

    return(JNI_TRUE);
}




gboolean
JavaLoadPlugin(gboolean initializing, GGobiPluginInfo *plugin)
{
    jboolean ans = JNI_TRUE;
    jmethodID mid;
    jclass klass;
    JNIEnv *env = std_env;

    klass = JVMENV FindClass(env, ((JavaInputPluginData*)plugin->data)->className);
    mid = JVMENV GetStaticMethodID(env, klass, "onLoad", "()Z");
    if(mid)
	ans = JVMENV CallStaticBooleanMethod(env, klass, mid);

    return(ans);
}

gboolean
JavaUnloadPlugin(gboolean initializing, GGobiPluginInfo *plugin)
{
    jboolean ans = JNI_TRUE;
    jmethodID mid;
    jclass klass;
    JNIEnv *env = std_env;

    klass = JVMENV FindClass(env, ((JavaInputPluginData*)plugin->data)->className);
    mid = JVMENV GetStaticMethodID(env, klass, "onUnload", "()Z");
    if(mid)
	ans = JVMENV CallStaticBooleanMethod(env, klass, mid);

    return(ans);
}


gboolean 
JavaCreatePlugin(ggobid *gg, GGobiPluginInfo *info, PluginInstance *inst)
{
    jboolean ans = JNI_TRUE;
    jclass klass;
    JavaInputPluginData *data = (JavaInputPluginData *) info->data;
    jmethodID cid;
    jobject obj;
    JNIEnv *env = std_env;
    jobject jgg;
    jmethodID ggId;

    data->runTime = g_malloc(sizeof(JavaRunTimeData));
    memset(data->runTime, '\0',sizeof(JavaRunTimeData));

    klass = JVMENV FindClass(env, data->className);

     /*  See if this class has a constructor that takes a single argument
         of class ggobi.ggobi.  If so, use that. Otherwise, use the default
         constructor (i.e. no arguments) and the call onCreate (?).
      */
    cid = JVMENV GetMethodID(env, klass, "<init>","(Lggobi/ggobi;)V");
    if(cid != NULL) {
	jclass tmpClass = JVMENV FindClass(env, "ggobi/ggobi");
        if(tmpClass == NULL) {
            if(JVMENV ExceptionOccurred(env))
		JVMENV ExceptionDescribe(env);
            return(JNI_FALSE);
	}
	ggId = JVMENV GetMethodID(env, tmpClass, "<init>","(D)V");
	jgg = JVMENV NewObject(env, tmpClass, ggId, (jdouble) (long) gg);    
	obj = JVMENV NewObject(env, klass, cid, jgg);
    } else {
	cid = JVMENV GetMethodID(env, klass, "<init>","()V");
	obj = JVMENV NewObject(env, klass, cid);
    }

    if(obj) {
        JavaPluginInstance *tmp;
        inst->data = tmp = g_malloc(sizeof(JavaPluginInstance));
        obj = JVMENV NewGlobalRef(env, obj);
	tmp->obj = obj;

        tmp->mids[GGobi_CLOSE] = JVMENV GetMethodID(env, klass, "onClose", "()Z");
        tmp->mids[GGobi_UPDATE_DISPLAY] = JVMENV GetMethodID(env, klass, "onUpdateDisplay", "()Z");
        if(tmp->mids[GGobi_UPDATE_DISPLAY]) {
            /* Register for events of updating the display menu. */           
	}
    } else {
        data->runTime = NULL;
	g_free(data->runTime);
    }

    return(ans);
}

gboolean 
JavaDestroyPlugin(ggobid *gg, GGobiPluginInfo *info, PluginInstance *inst)
{
    jboolean ans = JNI_TRUE;
    jobject obj;
    JavaPluginInstance *d = inst->data;
    JNIEnv *env = std_env;
    
    obj = d->obj;

    if(d->mids[GGobi_CLOSE]) {
	ans = JVMENV CallBooleanMethod(env, obj, d->mids[GGobi_CLOSE]);
    }

    JVMENV DeleteGlobalRef(env, obj);
    g_free(d);

    return(ans);
}


gboolean 
JavaUpdateDisplayMenu(ggobid *gg, PluginInstance *inst)
{

   JavaPluginInstance *d = inst->data;
   jmethodID mid;
   JNIEnv *env = std_env;
   jboolean ans = JNI_TRUE;

   if((mid = d->mids[GGobi_UPDATE_DISPLAY]) == NULL) {
       return(JNI_FALSE);
   } 

   ans = JVMENV CallBooleanMethod(env, d->obj, mid);
   return(ans);
}


/**

 */
jobject
runPlugin(const char * const klass, JNIEnv *env)
{

 jclass cls;
 jmethodID id;
 jobject obj;

   cls = (*env)->FindClass(env, klass); 
   if(!cls) {
       fprintf(stderr, "Cannot find class %s\n", klass);
       exit(3);
   }
   id = (*env)->GetMethodID(env, cls, "<init>", "()V");
   if(!id) {
       fprintf(stderr, "Cannot find method in class %s\n", klass);
       exit(3);
   }
   obj = (*env)->NewObject(env, cls, id);
   (*env)->DeleteLocalRef(env, cls);

   return(obj);
}
