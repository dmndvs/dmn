<?php
/**
 * structure form for column deletion
 * @param $form
 * @param $form_state
 * @return mixed
 */
function dmn_admin_column_delete_form($form, &$form_state)
{

    $form['dmn_title'] = array(
        '#type' => 'item',
        '#title' => '<h1>' . t('Delete existing column') . '</h1>'
    );

    $options = array();

    $result = db_query('SHOW COLUMNS FROM {dmn_table}');
    foreach ($result as $row) {
        $options[$row->Field] = $row->Field;
    };

    $form['dmn_fieldset'] = array(
        '#type' => 'fieldset',
        '#title' => t('checkboxes'),
        '#collapsible' => TRUE,
        '#collapsed' => FALSE,
    );

    $form['dmn_fieldset']['dmn_checkboxes'] = array(
        '#type' => 'checkboxes',
        '#title' => t('Choose columns for drop'),
        '#options' => $options,
    );

    $form['submit_button'] = array(
        '#type' => 'submit',
        '#value' => t('drop fields'),
        '#weight' => 10,
    );
    return $form;
}

/**
 * Validate handler for dmn_admin_column_delete_form
 * if base column are checked => uncheck
 *
 * @param $form
 * @param $form_state
 */
function dmn_admin_column_delete_form_validate($form, &$form_state){
    $syscolumn = array('dmnid', 'dmn_fname', 'dmn_lname', 'dmn_info', 'dmn_color');
    foreach($form_state['values']['dmn_checkboxes'] AS $key => $value) {
        if(in_array($key, $syscolumn) && $value != '0') {
            drupal_set_message(t('@column is system column', array('@column' => $form_state['values']['dmn_checkboxes'][$key])), 'error');
            $form_state['values']['dmn_checkboxes'][$key] = 0;
        }
    }
}

/**
 * submit handler for dmn_admin_column_delete_form
 * Drop checked columns
 *
 * @param $form
 * @param $form_state
 */

function dmn_admin_column_delete_form_submit($form, &$form_state)
{
    $dbquery = 'ALTER TABLE {drupal.dmn_table} DROP ';
    foreach ($form_state['values']['dmn_checkboxes'] as $cname) {
        if ($cname != '0') {
            $columns[] = $cname;
        }
    };
    if (isset($columns)) {
        $dbquery .= implode(', DROP ', $columns);
        $query = db_query($dbquery);
        if ($query) {
            drupal_set_message(t('@columns are dropped', array('@columns' => implode(', ', $columns))));
            cache_clear_all('*', 'dmn_cache', TRUE);
        }
    }



}