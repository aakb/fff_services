<?php

/**
 * @file
 * Holdes basic web-services logic and returns JSONP encode reponses based on
 * the input parmateres.
 *
 * It exposes 2 methods:
 *    getFact -> index.php?method=getFact&callback=parseJson&guid=2
 *    getFacts -> index.php?method=getFacts&offset=25&count=25&callback=parseJson
 *    getGuid -> index.php?method=getGuid&callback=parseJson
 *
 *
 * If an error is detected the following JSONP will be returned, so make sure to have
 * an error callback function.
 *
 *   error({"msg":"The GUID provided is not a number!","code":-1})
 */

include_once dirname(__FILE__).'/utils/utils.inc';
include_once dirname(__FILE__).'/database/fact.inc';

/**
 * Defines the methods avaliable.
 */
define('FFF_ERROR_CALLBACK', 'error');
define('FFF_GET_FACT', 'getfact');
define('FFF_GET_FACTS', 'getfacts');
define('FFF_GET_GUID', 'getguid');


/**
 * Helper function that parses the GET parameters into an options array.
 *
 * @return array $options
 *   The array contains as minimum the options 'method' and 'callback'.
 */
function parseOptions() {
  $options = array();
  // Try to get method and callback parameters.
  try {
    $options['method'] = Utils::getMethod();
    $options['callback'] = Utils::getCallback();
  } catch (Exception $e) {
    // The was an error in getting the parameters, so return an error message.
    returnJsonError(array(
      'msg' => $e->getMessage(),
      'code' => $e->getCode(),
    ));
  }

  // If getFact is called, try to get the guid form the URL.
  if ($options['method'] == FFF_GET_FACT) {
    try {
      $options['guid'] = Utils::getGUID();
    } catch (Exception $e) {
      // The was an error in getting the GUID, so return an error message.
      returnJsonError(array(
        'msg' => $e->getMessage(),
        'code' => $e->getCode(),
      ));
    }
  }

  if ($options['method'] == FFF_GET_FACTS) {
    try {
      $options['offset'] = Utils::getOffset();
      $options['count'] = Utils::getCount();
    } catch (Exception $e) {
      // The was an error in getting the offset, so return an error message.
      returnJsonError(array(
        'msg' => $e->getMessage(),
        'code' => $e->getCode(),
      ));
    }
  }

  return $options;
}

/**
 * Helper function to return an error message in JSONP with the error callback.
 *
 * @param array $result
 *   The array should contain the keys 'msg' and 'code'.
 */
function returnJsonError($result) {
   header('Content-type: application/json');
   echo FFF_ERROR_CALLBACK . '(' . json_encode($result) . ')';
   exit(isset($result['code']) ? $result['code'] : -1);
}

/**
 * Encodes the $result parameter into JSON and wraps it in the callback, it then
 * prints the result, thereby returning it to the user.
 *
 * @param array $result
 * @param string $callback
 */
function returnJson($result, $callback) {
   header('Content-type: application/json');
   echo $callback . '(' . json_encode($result) . ')';
}

// Main part of index, which takes action based on the options parsed by the
// helper functions above.
$options = parseOptions();
switch ($options['method']) {
  case FFF_GET_FACT:
    try {
      $fact = new Fact(array('guid' => $options['guid']));
      returnJson($fact->getFact(), $options['callback']);
    } catch (Exception $e) {
      returnJsonError(array(
        'msg' => $e->getMessage(),
        'code' => $e->getCode(),
      ));
    }
    exit(1);
    break;

  case FFF_GET_FACTS:
    try {
      $facts = Fact::getFacts($options['offset'], $options['count']);
      returnJson($facts, $options['callback']);
    } catch (Exception $e) {
      returnJsonError(array(
        'msg' => $e->getMessage(),
        'code' => $e->getCode(),
      ));
    }
    exit(1);
    break;

  case FFF_GET_GUID:
    returnJson(array('guid' => Fact::getGUID()), $options['callback']);
    exit(1);
    break;

  default:
    returnJsonError(array(
      'msg' => 'Method is not available',
      'code' => -1,
    ));
    exit(-1);
    break;
}
