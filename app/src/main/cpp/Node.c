/*
 *
 *   Copyright (C) 2018-2019 by C.H. Huang
 *   plushuang.tw@gmail.com
 */

#include <jni.h>
#include <stdint.h>
#include <stddef.h>
#include <pthread.h>

#include <UgString.h>
#include <UgetNode.h>
#include <UgetData.h>

#include <android/log.h>

#define LOG_TAG    "com.ugetdm.uget.lib"
#define LOGV(...)  __android_log_print( ANDROID_LOG_VERBOSE, LOG_TAG, __VA_ARGS__)
#define LOGD(...)  __android_log_print( ANDROID_LOG_DEBUG,   LOG_TAG, __VA_ARGS__)
#define LOGI(...)  __android_log_print( ANDROID_LOG_INFO,    LOG_TAG, __VA_ARGS__)
#define LOGW(...)  __android_log_print( ANDROID_LOG_WARN,    LOG_TAG, __VA_ARGS__)
#define LOGE(...)  __android_log_print( ANDROID_LOG_ERROR,   LOG_TAG, __VA_ARGS__)

// JNI static functions

JNIEXPORT jlong
Java_com_ugetdm_uget_lib_Node_create (JNIEnv* env, jclass nodeClass)
{
	return (jlong)(intptr_t) uget_node_new(NULL);
}

JNIEXPORT void
Java_com_ugetdm_uget_lib_Node_free (JNIEnv* env, jclass nodeClass, jlong pointer)
{
	uget_node_free((UgetNode*)(intptr_t) pointer);
}

JNIEXPORT jlong
Java_com_ugetdm_uget_lib_Node_base (JNIEnv* env, jclass nodeClass, jlong pointer)
{
    UgetNode*  node;

    node = (UgetNode*)(intptr_t) pointer;
    if (node)
        return (jlong)(intptr_t) node->base;
    else
        return 0;
}

JNIEXPORT jlong
Java_com_ugetdm_uget_lib_Node_next (JNIEnv* env, jclass nodeClass, jlong pointer)
{
	UgetNode*  node;

	node = (UgetNode*)(intptr_t) pointer;
	if (node)
		return (jlong)(intptr_t) node->next;
	else
		return 0;
}

JNIEXPORT jlong
Java_com_ugetdm_uget_lib_Node_prev (JNIEnv* env, jclass nodeClass, jlong pointer)
{
	UgetNode*  node;

	node = (UgetNode*)(intptr_t) pointer;
	if (node)
		return (jlong)(intptr_t) node->prev;
	else
		return 0;
}

JNIEXPORT jlong
Java_com_ugetdm_uget_lib_Node_parent (JNIEnv* env, jclass nodeClass, jlong pointer)
{
	UgetNode*  node;

	node = (UgetNode*)(intptr_t) pointer;
	if (node)
		return (jlong)(intptr_t) node->parent;
	else
		return 0;
}

JNIEXPORT jlong
Java_com_ugetdm_uget_lib_Node_children (JNIEnv* env, jclass nodeClass, jlong pointer)
{
	UgetNode*  node;

	node = (UgetNode*)(intptr_t) pointer;
	if (node)
		return (jlong)(intptr_t) node->children;
	else
		return 0;
}

JNIEXPORT jint
Java_com_ugetdm_uget_lib_Node_nChildren (JNIEnv* env, jclass nodeClass, jlong pointer)
{
    UgetNode*  node;

    node = (UgetNode*)(intptr_t) pointer;
    if (node)
		return node->n_children;
    else
		return 0;
}

JNIEXPORT jlong
Java_com_ugetdm_uget_lib_Node_data (JNIEnv* env, jclass nodeClass, jlong pointer)
{
	UgetNode*  node;

	node = (UgetNode*)(intptr_t) pointer;
	return (jlong)(intptr_t) node->info;
}

// Java_com_ugetdm_uget_lib_Node_getPosition
static jint getPosition (UgetNode* node, UgetNode* child)
{
	int  position;

	if (node == child->parent)
		return uget_node_child_position (node, child);

	for (child = child->fake;  child;  child = child->peer) {
		position = getPosition (node, child);
		if (position != -1)
			return position;
	}
	return -1;
}

JNIEXPORT jint
Java_com_ugetdm_uget_lib_Node_getPosition (JNIEnv* env, jclass nodeClass, jlong pointer, jlong childPointer)
{
	jint  result;

	result = getPosition ((UgetNode*)(intptr_t) pointer,
	                      (UgetNode*)(intptr_t) childPointer);

	return result;
}

JNIEXPORT jlong
Java_com_ugetdm_uget_lib_Node_getNthChild (JNIEnv* env, jclass nodeClass, jlong pointer, jint nth)
{
	UgetNode*  node;
	jlong      result;

	node = (UgetNode*)(intptr_t) pointer;
	if (node == NULL || nth == -1)
		result = 0L;
	else
		result = (jlong)(intptr_t) uget_node_nth_child (node, nth);

	return result;
}

JNIEXPORT jlong
Java_com_ugetdm_uget_lib_Node_getFakeByGroup (JNIEnv* env, jclass nodeClass, jlong pointer, jint group)
{
	UgetNode* node;

	node = (UgetNode*)(intptr_t) pointer;
    return (jlong)(intptr_t) uget_node_get_split(node, group);
}


// Java_com_ugetdm_uget_lib_Node_getFakeByParent
static UgetNode* getFakeByParent (UgetNode* node, UgetNode* parent)
{
	UgetNode*  temp;

	if (node->parent == parent)
		return node;

	for (node = node->fake;  node;  node = node->peer) {
		temp = getFakeByParent (node, parent);
		if (temp != NULL)
			return temp;
	}

	return NULL;
}

JNIEXPORT jlong
Java_com_ugetdm_uget_lib_Node_getFakeByParent (JNIEnv* env, jclass nodeClass, jlong pointer, jlong parentPointer)
{
	jlong     result;

	result = (intptr_t) getFakeByParent ((UgetNode*)(intptr_t) pointer,
	                                     (UgetNode*)(intptr_t) parentPointer);

	return result;
}
