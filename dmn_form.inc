<?php

/**
 * Structure of DMN form
 *
 * @return mixed
 * @param  $form
 * @name   vo_contact_form
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

  if (!empty($form_state['values']['dmn_checkbox'])) {
    $form['dmn_form']['dmn_info'] = array(
      '#type' => 'textarea',
      '#title' => t('Information:'),
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
    cache_clear_all(array('dmn:view', 'dmn:view-ann'), 'dmn_cache');
  }
  else {
    drupal_set_message(t('something went wrong'), 'error');
  };
}

function ajax_dmn_checkbox_callback($form, $form_state) {
  return $form['dmn_form'];
}

function ajax_dmn_rebuild_callback($form, $form_state) {
  $fname = $form_state['values']['dmn_fname'];
  $lname = $form_state['values']['dmn_lname'];
  if (empty(form_get_errors())) {
    drupal_get_messages();
    return array(
      '#type' => 'ajax',
      '#commands' => array(
        array(
          'command' => 'dmn_rebuild',
          'fname' => $fname,
          'lname' => $lname,
          'elementId' => 'dmn-fieldset',
        ),
      ),
    );
  }
  else {
    return array(
      '#type' => 'ajax',
      '#commands' => array(
        array(
          'command' => 'dmn_redirect_to_main',
        ),
      )
    );
  }
}

function ajax_dmn_redirect_callback($form, $form_state) {
  $_SESSION['dmn_fname'] = $form_state['values']['dmn_fname'];
  $_SESSION['dmn_lname'] = $form_state['values']['dmn_lname'];
  if (empty(form_get_errors())) {
    return array(
      '#type' => 'ajax',
      '#commands' => array(
        array(
          'command' => 'dmn_redirect',
        ),
      ),
    );
  }
  else {
    return array(
      '#type' => 'ajax',
      '#commands' => array(
        array(
          'command' => 'dmn_redirect_to_main',
        ),
      )
    );
  }
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

  if (!isset($form_state['values']['dmn_info'])) {
    $form_state['values']['dmn_info'] = '';
  }

  $setting = array(
    $form_state['values']['dmn_fname_color'],
    $form_state['values']['dmn_lname_color'],
  );
  //$dmncolor = serialize($setting); //serialize array for db_insert

  $valu = array(
    'dmn_fname' => $form_state['values']['dmn_fname'],
    'dmn_lname' => $form_state['values']['dmn_lname'],
    'dmn_info' => $form_state['values']['dmn_info'],
    'dmn_color' => $setting,
  );
  if (drupal_write_record('dmn_table', $valu)) {
    return TRUE;
  }
  else {
    return FALSE;
  }
  // use db_insert for inserting data
  /*
    $insert = db_insert('dmn_table')
      ->fields(array(
      'dmn_fname' => $form_state['values']['dmn_fname'],
      'dmn_lname' => $form_state['values']['dmn_lname'],
      'dmn_info' => $form_state['values']['dmn_info'],
      'dmn_color' => $dmncolor,
    ))
      ->execute();

    if ($insert) {
      return TRUE;
    }
    else {
      return FALSE;
    }
*/
}

function generateRandomString($length = 10) {
  $characters = '0123456789abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ';
  $charactersLength = strlen($characters);
  $randomString = '';
  for ($i = 0; $i < $length; $i++) {
    $randomString .= $characters[rand(0, $charactersLength - 1)];
  }
  return $randomString;
}