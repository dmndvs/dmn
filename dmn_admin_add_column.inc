<?php
/**
 * @file     dmn_admin_add_colums.inc
 * @category Testing
 * @return   mixed
 * @author   dmn_dvs <localhost@localhost.loc>
 *
 * dmn_admin_add_colums
 * administration for DMN module
 * define structure form for adding new column into table
 */

/**
 * Structure for admin form
 *
 * @param $form
 * @param $form_state
 * @return mixed
 */

function dmn_admin_column_add_form($form, &$form_state) {

    $options = array(
        'TEXT' => 'TEXT',
        'VARCHAR' => 'VARCHAR',
        'DATE' => 'DATE',
        'int' => 'INTEGER',
        'BOOLEAN' => 'BOOLEAN',
        'BLOB' => 'BLOB',
    );

    $form['dmn_title'] = array(
        '#type' => 'item',
        '#title' => '<h1>' . t('Add new column') . '</h1>'
    );

    $form['dmn_add_cname'] = array(
        '#type' => 'textfield',
        '#title' => t('Column Name'),
        '#maxlength' => 20,
        '#required' => TRUE,
    );

    $form['dmn_add_datatype'] = array(
        '#type' => 'select',
        '#title' => 'Column type:',
        '#options' => $options,
    );

    $form['dmn_add_datatype_length'] = array(
        '#type' => 'textfield',
        '#title' => t('datatype length:'),
        '#size' => 5,
        '#states' => array(
            'visible' => array(
                array(':input[name="dmn_add_datatype"]' => array('value' => 'VARCHAR')),
            ),
        ),
    );

    $form['submit_button'] = array(
        '#type' => 'submit',
        '#value' => t('add column'),
        '#weight' => 10,
    );

    return $form;
}

/**
 * Submit handler of form
 *
 * @param $form
 * @param $form_state
 */

function dmn_admin_column_add_form_submit($form, &$form_state) {
    $query = dmn_admin_column_add_db_query($form_state);
    if(isset($query)) {
        drupal_set_message(t('ok'));
        cache_clear_all('dmn:view', 'dmn_cache');
    }
}

/**
 * Database query
 * @param $form_state
 * @return DatabaseStatementInterface
 */

function dmn_admin_column_add_db_query($form_state) {
    $form_state['values']['dmn_add_cname'] = 'dmn_' . $form_state['values']['dmn_add_cname'];
    if(!empty($form_state['values']['dmn_add_datatype_length'])) {
        $form_state['values']['dmn_add_datatype'] .= '(' .$form_state['values']['dmn_add_datatype_length'].')';
    }
    elseif (empty($form_state['values']['dmn_add_datatype_length']) && $form_state['values']['dmn_add_datatype'] == 'VARCHAR')
    {
        $form_state['values']['dmn_add_datatype'] .= '(40)';
    }
    $dbquery = 'ALTER TABLE {drupal.dmn_table} ADD '.$form_state['values']['dmn_add_cname'] . ' ' . $form_state['values']['dmn_add_datatype'];

    return db_query($dbquery);
}