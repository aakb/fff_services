<?php

/**
 * @file
 * Util class that holds helper functions
 */

class Utils {

  /**
   * Gets a parameter from either $_GET or $_POST, if the parameter is not found
   * an exception is thrown.
   * @param type $param
   * @return type
   */
  public static function getParam($param) {
    if ($_GET[$param] != '') {
      return Utils::check_plain($_GET[$param]);
    }
    else if ($_POST[$param] != '') {
      return Utils::check_plain($_POST[$param]);
    }
    else {
      throw new Exception('Parameter not found: '. $param);
    }
  }

  public static function isAjax() {
    if(!empty($_SERVER['HTTP_X_REQUESTED_WITH']) && strtolower($_SERVER['HTTP_X_REQUESTED_WITH']) == 'xmlhttprequest') {
      return TRUE;
    }
    return FALSE;
  }

  public static function getMethod() {
    try {
      $method = strtolower(Utils::check_plain(Utils::getParam('method')));
    } catch (Exception $e) {
      throw new Exception('Unable to find the method parameter in the URL!', -1);
    }
    return $method;
  }

  public static function getCallback() {
    try {
      $callback = strtolower(Utils::check_plain(Utils::getParam('callback')));
    } catch (Exception $e) {
      throw new Exception('Unable to find the callback parameter in the URL!', -1);
    }
    return $callback;
  }

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

  public static function check_plain($text) {
    return htmlspecialchars($text, ENT_QUOTES, 'UTF-8');
  }
}

?>
