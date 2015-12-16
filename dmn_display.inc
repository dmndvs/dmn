<?php
/**
 * Created by PhpStorm.
 * User: dmn_dvs
 * Date: 09.12.15
 * Time: 15:05
 */

/**
 * Create new view with all data from database
 * for admin: all columns
 * for anonymous: all, except dmn_color
 */
function dmn_display() {

  $content = '';
  global $user;
  if(isset($user->roles[1])) {
    $cid = 'dmn:view-ann';
  }
  else
  {
    $cid = 'dmn:view';
  }
  $cache = cache_get($cid, 'dmn_cache');
  if(empty($cache) || time() > $cache->expire) {
  $header = array();
  $column = db_query('SHOW COLUMNS FROM {dmn_table}');
  foreach ($column as $row) {
    if(isset($user->roles[1]) && $row->Field == 'dmn_color') {

    }
    else {
      $header[] = $row->Field;
    }
  };
  $rows = array();
  $result = db_query('SELECT * FROM {dmn_table}');
  foreach ($result as $row) {
    $data = array();
    foreach ($row as $key => $value) {
      if(isset($user->roles[1]) && $key == 'dmn_color') {
        if (isset($value)) {
          if (isset($data['dmn_fname'])) {
            $data['dmn_fname']['style'] = 'color:' . unserialize($value)[0];
          }
          if (isset($data['dmn_lname'])) {
            $data['dmn_lname']['style'] = 'color:' . unserialize($value)[1];
          }
        }
      }
      else
      {
        $data[$key] = array(
          'data' => $value,
        );
        if ($key == 'dmn_color' && isset($value)) {
          if (isset($data['dmn_fname'])) {
            $data['dmn_fname']['style'] = 'color:' . unserialize($value)[0];
          }
          if (isset($data['dmn_lname'])) {
            $data['dmn_lname']['style'] = 'color:' . unserialize($value)[1];
          }
        }
      }
    }

    $rows[] = $data;
  }
    $cache = array(
      'header' => $header,
      'rows' => $rows,
    );

    cache_set($cid, $cache, 'dmn_cache', REQUEST_TIME  + (60 * 30));
  }
  else
  {
    drupal_set_message(t('cached version'));
    $header = $cache->data['header'];
    $rows = $cache->data['rows'];
  }

  $content = theme('table', array(
    'header' => $header,
    'rows' => $rows,
    'attributes' => array(
      'class' => array('dmn')
    ),
    '#empty' => t('Your table is empty'),
  ));

  return $content;
}

/**
 * view 2, testing, not use
 * @return string
 */

function dmn_display_2() {
  global $user;
  $cid = 'dmn:view';
  foreach($user->roles AS $role) {
    switch ($role) {
      case 'anonymous user':
        $cid .= ':anonymous';
        break;
      case 'authenticated user':
        $cid .= ':authenticated';
        break;
      case 'administrator':
        $cid .= ':admin';
        break;
      case 'admin':
        $cid .= ':admin';
        break;
    }
  }
  $header = array();
  $rows = array();

  $output = '';
  $output .= $cid;

  return $output;
}
