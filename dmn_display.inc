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
function dmn_display()
{
    global $user;
    $header = array();
    $rows = array();
    $output = '';
    $cid = 'dmn:view';
    $fields = array('dmn_fname', 'dmn_lname', 'dmn_color');
    foreach ($user->roles AS $role) {
        switch ($role) {
            case 'anonymous user':
                $cid .= ':anonymous';
                $fields = array('dmn_fname', 'dmn_lname', 'dmn_color');
                break;
            case 'authenticated user':
                $cid .= ':authenticated';
                $fields = array('dmn_fname', 'dmn_lname', 'dmn_info', 'dmn_color');
                break;
            case 'administrator':
                $cid .= ':admin';
                $fields = array();
                break;
            case 'admin':
                $cid .= ':admin';
                $fields = array();
                break;
        }
    }
    $cache = cache_get($cid, 'dmn_cache');
    if (empty($cache)) {
        $query = db_select('dmn_table', 'dmn')
          ->fields('dmn', $fields)
          ->execute();
        while ($result = $query->fetchAssoc()) {
            foreach ($result AS $key => $value) {
                if (!in_array($key, $header)) {
                    $header[] .= $key;
                }
                $data[$key] = array(
                  'data' => $value,
                );
            }
            if (isset($result['dmn_color'])) {
                $color = unserialize($result['dmn_color']);
                $data['dmn_fname']['style'] = 'color:' . $color['fname'];
                $data['dmn_lname']['style'] = 'color:' . $color['lname'];
            }
            $rows[] = $data;
        }
        $cache = array(
          'header' => $header,
          'rows' => $rows,
        );
        cache_set($cid, $cache, 'dmn_cache');
    }
    else
    {
        drupal_set_message(t('cached version'));
        $header = $cache->data['header'];
        $rows = $cache->data['rows'];
    }
    $output = theme('table', array(
      'header' => $header,
      'rows' => $rows,
      'attributes' => array(
        'class' => array('dmn')
      ),
      '#empty' => t('Your table is empty'),
    ));
    if(empty($header)) {
        $output = '<p>' . t('No data to show') . '</p>';
    }
    return $output;
}
/*
/**
 * view 2, testing, not use
 * @return string
 */
/*
function dmn_display_()
{

}
*/