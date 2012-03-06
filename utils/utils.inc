<?php

/**
 * @file
 * Util class that holds helper functions
 */

class Utils {

  /**
   * Gets a parameter from either $_GET or $_POST, if the parameter is not found
   * an exception is thrown.
   *
   * @param string $param
   *   The name of the parameter that should be parsed.
   * @return string
   *   The value of the parameter.
   */
  public static function getParam($param) {
    if ($_GET[$param] != '') {
      return Utils::check_plain($_GET[$param]);
    }
    else if ($_POST[$param] != '') {
      return Utils::check_plain($_POST[$param]);
    }
    else {
      // The parameter was not found.
      throw new Exception('Parameter not found: '. $param);
    }
  }

  /**
   * Checks is the request is a xml http request (AJAX/XHR).
   *
   * @return bool
   *   TRUE if it's a XHR call else FALSE.
   */
  public static function isAjax() {
    if(!empty($_SERVER['HTTP_X_REQUESTED_WITH']) && strtolower($_SERVER['HTTP_X_REQUESTED_WITH']) == 'xmlhttprequest') {
      return TRUE;
    }
    return FALSE;
  }

  /**
   * Gets the method parameter.
   *
   * @return string
   *   The method name
   */
  public static function getMethod() {
    try {
      $method = strtolower(Utils::check_plain(Utils::getParam('method')));
    } catch (Exception $e) {
      throw new Exception('Unable to find the method parameter in the URL!', -1);
    }
    return $method;
  }

  /**
   * Gets the callback parameter.
   *
   * @return string
   *   The callback name.
   */
  public static function getCallback() {
    try {
      $callback = Utils::check_plain(Utils::getParam('callback'));
    } catch (Exception $e) {
      throw new Exception('Unable to find the callback parameter in the URL!', -1);
    }
    return $callback;
  }

  /**
   * Gets the GUID parameter.
   *
   * @return int
   *   The guid parsed from the parameter.
   */
  public static function getGUID() {
    try {
      $guid = Utils::check_plain(Utils::getParam('guid'));
    } catch (Exception $e) {
      throw new Exception('Unable to find the GUID parameter in the URL!', -1);
    }

    if (!is_numeric($guid) || $guid < 0) {
      throw new Exception('The GUID provided is not a number!', -1);
    }

    return $guid;
  }

  /**
   * Sanitizes the input string to remove evil code.
   *
   * @param string $text
   * @return string
   *   Sanitized string
   */
  public static function check_plain($text) {
    return htmlspecialchars($text, ENT_QUOTES, 'UTF-8');
  }
}