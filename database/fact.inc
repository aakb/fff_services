<?php

/**
 * @file
 * Representation of a database fact as an object.
 */

include_once dirname(__FILE__).'/pdo_mysql.inc';

class Fact {

  private $connection = NULL;
  private $properties = array();

  public function __construct($values = array()) {
    $this->properties = $values + array(
      'guid' => NULL, // Node nid from Drupal
      'title' => NULL,
      'author' => NULL,
      'time' => NULL,
      'content' => NULL,
      'inspirations' => array(),
      'sources' => array(),
      'keywords' => array(),
    );

    $this->connection = PDOMysql::getInstance();
    $this->loadFact();
  }

  /**
   * Implementation of setter methode.
   */
  public function __set($key, $val) {
    if (array_key_exists($key, $this->properties)) {
      $this->properties[$key] = $val;
    }
    else {
      throw new Exception('Property "'.$key.'" does not exists in '.basename(__FILE__).' line '.__LINE__);
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
      throw new Exception('Property "'.$key.'" does not exists in '.basename(__FILE__).' line '.__LINE__);
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

  public function toJson() {
    if ($this->guid == NULL) {
      throw new Exception('The fact have not been loaded from the database!', -1);
    }
    return $this->properties;
  }

  private function loadKeywords() {
    $keywords = array();
    $statement = "SELECT terms.name AS keyword
                    FROM node node
              INNER JOIN field_revision_field_fact_tags tags ON node.nid = tags.entity_id
              INNER JOIN taxonomy_term_data terms ON tags.field_fact_tags_tid = terms.tid
                   WHERE node.type = 'fact' AND node.nid = " . $this->guid;
    $query = $this->connection->execute($statement);
    while ($row = $query->fetch(PDO::FETCH_ASSOC)) {
      $keywords[] = $row['keyword'];
    }
    $this->keywords = $keywords;
  }

  private function loadSources() {
    $sources = array();
    $statement = "SELECT sources.field_fact_source_title AS title,
                         sources.field_fact_source_url AS url
                    FROM node node
              INNER JOIN field_revision_field_fact_source sources ON node.nid = sources.entity_id
                   WHERE node.type = 'fact' AND node.nid = " . $this->guid;
    $query = $this->connection->execute($statement);
    while ($row = $query->fetch(PDO::FETCH_ASSOC)) {
      $sources[] = $row;
    }
    $this->sources = $sources;
  }

  private function loadInspirations() {
    $inspirations = array();
    $statement = "SELECT inspirations.field_fact_inspiration_title AS title,
                         inspirations.field_fact_inspiration_url AS url
                    FROM node node
              INNER JOIN field_revision_field_fact_inspiration inspirations ON node.nid = inspirations.entity_id
                   WHERE node.type = 'fact' AND node.nid = " . $this->guid;
    $query = $this->connection->execute($statement);
    while ($row = $query->fetch(PDO::FETCH_ASSOC)) {
      $inspirations[] = $row;
    }
    $this->inspirations = $inspirations;
  }

  private function loadFact($random = FALSE) {
    // Select nodes where workflow is publised.
    $statement = "SELECT node.title AS title,
                         node.changed AS time,
                         author.field_fact_author_value AS author,
                         content.field_fact_content_value AS content
                    FROM node node
              INNER JOIN field_data_field_fact_author author ON node.nid = author.entity_id
              INNER JOIN field_data_field_fact_content content ON node.nid = content.entity_id
              INNER JOIN field_data_field_workflow workflow ON node.nid = workflow.entity_id
                     AND (workflow.entity_type = 'node' AND workflow.deleted = '0')
                   WHERE node.type = 'fact'
                     AND workflow.field_workflow_tid = '2'";

//      'inspiration' => NULL,
//      'source' => NULL,

    if ($random) {
     $statement .= ' ORDER BY rand() LIMIT 1';
    }
    else {
      $statement .= ' AND node.nid = ' . $this->guid;
    }
    $query = $this->connection->execute($statement);

    // Fetch and insert data into this ovject
    $row = $query->fetch(PDO::FETCH_ASSOC);
    if (!empty($row)) {
      foreach ($row as $key => $value) {
        $this->$key = $value;
      }
    }
    else {
      throw new Exception('Fact with guid "'.$this->guid.'" not found in '.basename(__FILE__).' line '.__LINE__);
    }

    $this->loadKeywords();
    $this->loadSources();
    $this->loadInspirations();
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
