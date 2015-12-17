<?php
/**
 * Created by PhpStorm.
 * User: dmn_dvs
 * Date: 15.12.15
 * Time: 14:39
 */
function dmn_view2_form($form, &$form_state) {
  $form = array();

  if(!empty($_SESSION['dmn_fname'])) {
    $defvalue = $_SESSION['dmn_fname'];
    unset($_SESSION['dmn_fname']);
  }
  else {
    $defvalue = '';
  }
  if(!empty($_SESSION['dmn_lname'])) {
    unset($_SESSION['dmn_lname']);
  }

  $form['name'] = array(
    '#title' => t('Your Name'),
    '#type' => 'textfield',
    '#default_value' => $defvalue,
  );

  // Blank output field which we will fill using AJAX.
  $form['output'] = array(
    '#prefix' => '<div id="output">',
    '#suffix' => '</div>',
    '#markup' => '',
  );

  // AJAX-enabled submit button.
  $form['submit'] = array(
    '#type' => 'submit',
    '#value' => t('Submit'),
    '#ajax' => array(
      'callback' => 'dmn_view2_form_ajax_callback',
    ),
  );
  return $form;
}


function dmn_view2_form_submit($form, $form_state) {
}

/**
 * AJAX callback function for dmn_view2_form().
 */
function dmn_view2_form_ajax_callback($form, $form_state) {
  // Grab the name input by the user.
  $name = $form_state['values']['name'];

  // Invoke out Special Effects ajax command for the special effect we want to create.
  $commands = array();
  $commands[] = array(
    'command' => 'special_effects',
    'elementId' => 'output',
    'name' => $name,
    'duration' => 4000,
    'amplify' => 3,
  );

  // Send the command to the page.
  $page = array(
    '#type' => 'ajax',
    '#commands' => $commands,
  );
  return $page;
}

