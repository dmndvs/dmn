<?php
/**
 * Structure of DMN form
 *
 * @return mixed
 * @param  $form
 * @name   dmn_form
 */

function dmn_form($form, &$form_state) {
  $form['dmn_form'] = array(
    '#title' => t("dmn_form"),
    '#prefix' => '<div id="dmn-fieldset">',
    '#suffix' => '</div>',
    '#type' => 'fieldset',
  );

  $form['dmn_form']['dmn_fname'] = array(
    '#type' => 'dmn_textfield',
    '#title' => t('First Name'),
    '#maxlength' => 20,
    '#required' => FALSE,
    //'#default_value' => generateRandomString(),
  );

  $form['dmn_form']['dmn_fname_color'] = array(
    '#type' => 'radios',
    '#default_value' => 'black',
    '#options' => array(
      'black' => 'black',
      'red' => 'red',
      'blue' => 'blue',
      'green' => 'green',
    ),
    '#description' => t('font color of First Name'),
  );

  $form['dmn_form']['dmn_lname'] = array(
    '#type' => 'dmn_textfield',
    '#title' => t('Last Name'),
    '#maxlength' => 20,
    '#required' => FALSE,
    //'#default_value' => generateRandomString(),

  );

  $form['dmn_form']['dmn_lname_color'] = array(
    '#type' => 'radios',
    '#default_value' => 'black',
    '#options' => array(
      'black' => 'black',
      'red' => 'red',
      'blue' => 'blue',
      'green' => 'green',
    ),
    '#description' => t('font color of Last Name'),
  );

  $form['dmn_form']['dmn_checkbox'] = array(
    '#type' => 'checkbox',
    '#title' => t('I have additional information'),
    '#ajax' => array(
      'wrapper' => 'dmn-fieldset',
      'callback' => 'ajax_dmn_checkbox_callback',
      'effect' => 'fade',
    ),
  );

  $form['dmn_form']['submit_button'] = array(
    '#type' => 'submit',
    '#value' => t('submit-redirect'),
    '#weight' => 10,
    '#ajax' => array(
      'callback' => 'ajax_dmn_redirect_callback',
    ),
  );
  $form['dmn_form']['submit_button2'] = array(
    '#type' => 'submit',
    '#value' => t('submit-rebuild'),
    '#weight' => 11,
    '#ajax' => array(
      'callback' => 'ajax_dmn_rebuild_callback',
    ),
  );
  $form['dmn_form']['submit_button3'] = array(
      '#type' => 'submit',
      '#value' => t('submit'),
      '#weight' => 12,
  );

  if (!empty($form_state['values']['dmn_checkbox'])) {
    $form['dmn_form']['dmn_info'] = array(
      '#type' => 'textarea',
      '#title' => t('Information:'),
      '#default_value' => '',
    );
  }

  $form['dmn_form']['dmn_email'] = array(
    '#type' => 'dmn_email',
    '#title' => 'Email',
  );
  return $form;
}

/**
 * Form validation handler for DMN_form().
 *
 * @param $form
 * @param $form_state
 * @form  DMN_form_validate()
 */
function dmn_form_validate($form, &$form_state) {
  if (!isset($form_state['values']['dmn_info'])) {
    $form_state['values']['dmn_info'] = '';
  }
  if (empty($form_state['values']['dmn_fname'])) {
    form_set_error('dmn_fname', 'empty first name');
  }
  if (empty($form_state['values']['dmn_lname'])) {
    form_set_error('dmn_lname', 'empty last name');
  }
}

/**
 * Form submission handler for DMN_form()
 *
 * @param $form
 * @param $form_state
 * @see   DMN_form_submit()
 */
function dmn_form_submit($form, &$form_state) {
  if (dmn_db_insert($form_state) === TRUE) {
    drupal_set_message(t('@fname @lname, successfully added', array(
      '@fname' => $form_state['values']['dmn_fname'],
      '@lname' => $form_state['values']['dmn_lname'],
    )));
    cache_clear_all('*', 'dmn_cache', TRUE);
  }
  else {
    drupal_set_message(t('something went wrong'), 'error');
  };
}

/**
 * callback function on checked dmn_checkbox
 *
 * @param $form
 * @param $form_state
 * @return mixed
 */
function ajax_dmn_checkbox_callback($form, $form_state) {
  return $form['dmn_form'];
}

/**
 * callback function on submit-rebuild button
 *
 * @param $form
 * @param $form_state
 * @return array
 */
function ajax_dmn_rebuild_callback($form, $form_state) {
  if (empty(form_get_errors())) {
    $fname = $form_state['values']['dmn_fname'];
    $lname = $form_state['values']['dmn_lname'];
    drupal_get_messages();
    $commands[] = array(
      'command' => 'dmn_rebuild',
      'fname' => $fname,
      'lname' => $lname,
      'elementId' => 'dmn-fieldset',
    );
  }
  else {
    $commands[] = array(
      'command' => 'dmn_redirect',
      'topath' => url('/dmn'),
    );
  }
  return array(
    '#type' => 'ajax',
    '#commands' => $commands,
  );
}

/**
 * callback function on submit-redirect button
 *
 * @param $form
 * @param $form_state
 * @return array
 */

function ajax_dmn_redirect_callback($form, $form_state) {
  $commands = array();
  if (empty(form_get_errors())) {
    $_SESSION['dmn_fname'] = $form_state['values']['dmn_fname'];
    $_SESSION['dmn_lname'] = $form_state['values']['dmn_lname'];
    $commands[] = array(
      'command' => 'dmn_redirect',
      'topath' => url('/dmn/view2'),
    );
  }
  else {
    $commands[] = array(
      'command' => 'dmn_redirect',
      'topath' => url('/dmn'),
    );
  }
  return array(
    '#type' => 'ajax',
    '#commands' => $commands,
  );
}

/**
 * Handler for insert into database
 *
 * @param $empty - it's count of empty fields vo_contact form (max = 4)
 * @param $form
 * @return bool
 * @throws \Exception
 * @see    dmn_db_insert()
 */

function dmn_db_insert(&$form_state) {

  $setting = array(
    'fname' => $form_state['values']['dmn_fname_color'],
    'lname' => $form_state['values']['dmn_lname_color'],
  );

  $data = array(
    'dmn_fname' => $form_state['values']['dmn_fname'],
    'dmn_lname' => $form_state['values']['dmn_lname'],
    'dmn_info' => $form_state['values']['dmn_info'],
    'dmn_color' => $setting,
  );
  if (drupal_write_record('dmn_table', $data)) {
    return TRUE;
  }
  else {
    return FALSE;
  }
}

/**
 * Generate random string for #default_value ;)
 *
 * @param int $length
 * @return string
 */
function generateRandomString($length = 10) {
  $characters = '0123456789abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ';
  $charactersLength = strlen($characters);
  $randomString = '';
  for ($i = 0; $i < $length; $i++) {
    $randomString .= $characters[rand(0, $charactersLength - 1)];
  }
  return $randomString;
}