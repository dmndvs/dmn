<?php
/**
 * dmn_admin.inc
 * administration for DMN module
 *
 * PHP version 5.3
 *
 * @file     dmn_admin
 * @category Testing
 * @return   mixed
 * @author   dmn_dvs <localhost@localhost.loc>
 */


/**
 * Structure for admin form on hook admin/config/content/dmn/ (alias = admin/config/content/dmn/drop)
 * Creation checkboxes for existing columns of a table module
 *
 * @param $form
 * @param $form_state
 * @return mixed
 */
function dmn_admin_column_delete_form($form, &$form_state) {

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
 * submit for form
 * It resets the marked columns
 * The System columns that defined in dmn.install file will not removed
 * @param $form
 * @param $form_state
 */
function dmn_admin_column_delete_form_submit($form, &$form_state){
  $dbquery = 'ALTER TABLE {drupal.dmn_table} DROP ';
  if($form_state['values']['dmn_checkboxes']['dmnid'] != '0') {
    drupal_set_message(t('@column is system column', array('@column' => $form_state['values']['dmn_checkboxes']['dmnid'])), 'error');
    $form_state['values']['dmn_checkboxes']['dmnid'] = 0;
  }
  if($form_state['values']['dmn_checkboxes']['dmn_fname'] != '0') {
    drupal_set_message(t('@column is system column', array('@column' => $form_state['values']['dmn_checkboxes']['dmn_fname'])), 'error');
    $form_state['values']['dmn_checkboxes']['dmn_fname'] = 0;
  }
  if($form_state['values']['dmn_checkboxes']['dmn_lname'] != '0') {
    drupal_set_message(t('@column is system column', array('@column' => $form_state['values']['dmn_checkboxes']['dmn_lname'])), 'error');
    $form_state['values']['dmn_checkboxes']['dmn_lname'] = 0;
  }
  if($form_state['values']['dmn_checkboxes']['dmn_info'] != '0') {
    drupal_set_message(t('@column is system column', array('@column' => $form_state['values']['dmn_checkboxes']['dmn_info'])), 'error');
    $form_state['values']['dmn_checkboxes']['dmn_info'] = 0;
  }
  if($form_state['values']['dmn_checkboxes']['dmn_color'] != '0') {
    drupal_set_message(t('@column is system column', array('@column' => $form_state['values']['dmn_checkboxes']['dmn_color'])), 'error');
    $form_state['values']['dmn_checkboxes']['dmn_color'] = 0;
  }
  foreach($form_state['values']['dmn_checkboxes'] as $cname) {
    if($cname != '0') {
      $columns[] = $cname;
    }
  };
  if (isset($columns)) {
    $dbquery .= implode(', DROP ', $columns);
    $query = db_query($dbquery);
    if($query) {
      drupal_set_message(t('@columns are dropped', array('@columns' => implode(', ', $columns))));
    }
  }


}


/**
 * Structure for admin form on hook admin/config/content/dmn/addfield
 * Generate new form for add column to the module table
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
        array(':input[name="dmn_add_datatype"]' => array('value' => 'BLOB')),
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
 * insert new column in the module table
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

/**
 * Generate view for admin page
 *
 * @return string
 * @throws \Exception
 */

function dmn_admin_view_page() {
  $content = '';
  $header = array('del', 'edit');
  $column = db_query('SHOW COLUMNS FROM {dmn_table}');
  foreach ($column as $row) {
    $header[] = $row->Field;
  };
  $rows = array();
  $result = db_query('SELECT * FROM {dmn_table}');
  foreach ($result as $row) {
    $data = array();
    $data['del'] = '<a href="' . url('admin/config/content/dmn/' . $row->dmnid . '/delete') . '">del</a>';
    $data['edit'] = '<a href="' . url('admin/config/content/dmn/editfields/' . $row->dmnid) . '">edit</a>';
    foreach ($row as $key => $value) {
      $data[$key] = array(
        'data' => $value,
      );
      if($key == 'dmn_color') {
        if(isset($data['dmn_fname'])){
          $data['dmn_fname']['style'] = 'color:' . unserialize($value)[0];
        }
        if(isset($data['dmn_lname'])){
          $data['dmn_lname']['style'] = 'color:' . unserialize($value)[1];
        }
      }
    }

    $rows[] = $data;

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
 * Structure for form, that provided possibility for update data in database
 *
 * @param $form
 * @param $form_state
 * @return mixed
 */

function dmn_edit_page($form, &$form_state) {

  $result = db_select('dmn_table', 'dmn')
    ->fields('dmn')
    ->condition('dmnid', $form_state['build_info']['args'][0],'=')
    ->execute()
    ->fetchAssoc();

  $form['dmn_fname'] = array(
    '#type' => 'dmn_textfield',
    '#title' => t('First Name'),
    '#maxlength' => 20,
    '#required' => FALSE,
    '#default_value' => $result['dmn_fname'],
  );

  $form['dmn_fname_color'] = array(
    '#type' => 'radios',
    '#default_value' => unserialize($result['dmn_color'])[0],
    '#options' => array(
      'black' => 'black',
      'red' => 'red',
      'blue' => 'blue',
      'green' => 'green',
    ),
    '#description' => t('font color of First Name'),
  );

  $form['dmn_lname'] = array(
    '#type' => 'dmn_textfield',
    '#title' => t('Last Name'),
    '#maxlength' => 20,
    '#required' => FALSE,
    '#default_value' => $result['dmn_lname'],
  );

  $form['dmn_lname_color'] = array(
    '#type' => 'radios',
    '#default_value' => unserialize($result['dmn_color'])[1],
    '#options' => array(
      'black' => 'black',
      'red' => 'red',
      'blue' => 'blue',
      'green' => 'green',
    ),
    '#description' => t('font color of Last Name'),
  );

  $form['dmn_checkbox'] = array(
    '#type' => 'dmn_checkbox',
    '#title' => t('I have additional information'),
  );

  $form['submit_button'] = array(
    '#type' => 'submit',
    '#value' => t('update'),
    '#weight' => 10,
  );

  $form['dmn_info'] = array(
    '#type' => 'textarea',
    '#title' => t('Information:'),
    '#states' => array(
      'visible' => array(
        ':input[name="dmn_checkbox"]' => array('checked' => TRUE),
      ),
    ),
    '#default_value' => $result['dmn_info'],
  );
  $form['dmn_id'] = array(
    '#type' => 'hidden',
    '#value' => $form_state['build_info']['args'][0],
  );

  return $form;
}

function dmn_dmn_edit_page_validate($form, &$form_state) {

}
/**
 * "Submit logic" for updating data in database
 *
 * @param $form
 * @param $form_state
 */
function dmn_edit_page_submit($form, &$form_state) {
  $update = dmn_edit_page_db_update($form_state);
  if(isset($update)) {
    drupal_set_message(t('values updated'));
    cache_clear_all('dmn:view', 'dmn_cache');;
    drupal_goto(url('admin/config/content/dmn/editfields'));
  }
  else {
    drupal_set_message(t('something went wrong'), 'error');
  }
}

function dmn_edit_page_db_update($form_state) {
  $dmn_color = array();
  $dmn_color[] .= $form_state['values']['dmn_fname_color'];
  $dmn_color[] .= $form_state['values']['dmn_lname_color'];
  $dmn_color = serialize($dmn_color);

  if(db_update('dmn_table')
    ->fields(array(
      'dmn_fname' => $form_state['values']['dmn_fname'],
      'dmn_lname' => $form_state['values']['dmn_lname'],
      'dmn_info' => $form_state['values']['dmn_info'],
      'dmn_color' => $dmn_color,
    ))
    ->condition('dmnid', $form_state['values']['dmn_id'])
    ->execute())
  {
    return TRUE;
  }
  else
  {
    return FALSE;
  }
}

function dmn_delete($id) {
  $delete = db_delete('dmn_table')
    ->condition('dmnid', $id)
    ->execute();
  if (isset($delete)) {
    drupal_set_message(t('deleted'));
    cache_clear_all('dmn:view', 'dmn_cache');
  }
  else {
    drupal_set_message(t('something went wrong!'), 'error');
  }
  drupal_goto(url('admin/config/content/dmn/editfields'));
}