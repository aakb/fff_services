<?php

/**
 * @file
 * Representation of a database fact as an object.
 */

include_once dirname(__FILE__).'/pdo_mysql.inc';

class Fact {

  private $table = 'facts';
  private $connection = NULL;
  private $properties = array();

  public function __construct($values = array()) {
    $this->properties = $values + array(
      'guid' => -1, // Node nid from Drupal
      'title' => NULL,
      'time' => NULL,
      'content' => NULL,
      'inspiration' => NULL,
      'source' => NULL,
      'keywords' => NULL,
    );
    
    $this->connection = PDOMysql::getInstance();
  }

  /**
   * Implementation of setter methode.
   */
  public function __set($key, $val) {
    if (array_key_exists($key, $this->properties)) {
      $this->properties[$key] = $val;
    }
    else {
      throw new Exception('Property "'.$key.'" does not exists in '.__FILE__.' line '.__LINE__);
    }
  }

  /**
   * Implementation of getter methode.
   */
  public function __get($key) {
    if (array_key_exists($key, $this->properties)) {
      return $this->properties[$key];
    }
    else {
      throw new Exception('Property "'.$key.'" does not exists in '.__FILE__.' line '.__LINE__);
    }
  }

  /**
   * Implements __toString() that returns the object formatted as an unordered
   * list.
   */
  public function __toString() {    
    $output = '<ul>';
    foreach ($this->properties as $key => $value) {
      $output .= '<li>'.$key.': '.$value.'</li>';
    }
    $output .= '</ul>';
    return $output;
  }

  public function load($guid = NULL, $random = FALSE) {
    // Select nodes where workflow is publised.
    $statement = "SELECT node.nid AS nid, 
                         node.title AS title,
                         node.changed AS changed,
                         author.field_fact_author_value AS author,
                         content.field_fact_content_value AS content
                    FROM node node
              INNER JOIN field_data_field_fact_author author ON node.nid = author.entity_id
              INNER JOIN field_data_field_fact_content content ON node.nid = content.entity_id
              INNER JOIN field_data_field_workflow workflow ON node.nid = workflow.entity_id
                     AND (workflow.entity_type = 'node' AND workflow.deleted = '0')
                   WHERE node.type = 'fact'
                     AND workflow.field_workflow_tid = '2'";
    if ($random) {
     $statement .= ' ORDER BY rand() LIMIT 1';
    }
    else if ($guid != NULL) {
      $statement .= ' node.nid = ' . $guid;
    }
    $query = $this->connection->execute($statement);

    // Fetch and insert data into this ovject
    $row = $query->fetch(PDO::FETCH_ASSOC);
    if (!empty($row)) {
      $this->properties = $row;
    }
    else {
      throw new Exception('Fact with guid "'.$this->guid.'" not found in '.__FILE__.' line '.__LINE__);
    }
  }

  public static function getGUID() {
    $connection = PDOMysql::getInstance();

    $statement = "SELECT node.nid AS GUID
                    FROM node node
              INNER JOIN field_data_field_workflow workflow ON node.nid = workflow.entity_id
                     AND (workflow.entity_type = 'node' AND workflow.deleted = '0')
                   WHERE node.type = 'fact' AND workflow.field_workflow_tid = '2'
                ORDER BY rand() LIMIT 1";
    $query = $connection->execute($statement);
    $row = $query->fetch(PDO::FETCH_ASSOC);
    if (!empty($row)) {
      return $row['GUID'];
    }
    else {
      throw new Exception('Fact with guid "'.$this->guid.'" not found in '.__FILE__.' line '.__LINE__);
    }
  }
}
