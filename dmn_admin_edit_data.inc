<?php
function dmn_admin_view_page()
{
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
            if ($key == 'dmn_color' && isset($value)) {
                $value = unserialize($value);
                if (isset($data['dmn_fname'])) {
                    $data['dmn_fname']['style'] = 'color:' . $value['fname'];
                }
                if (isset($data['dmn_lname'])) {
                    $data['dmn_lname']['style'] = 'color:' . $value['lname'];
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

function dmn_edit_page($form, &$form_state)
{

    $result = db_select('dmn_table', 'dmn')
        ->fields('dmn')
        ->condition('dmnid', $form_state['build_info']['args'][0], '=')
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
        '#default_value' => unserialize($result['dmn_color'])['fname'],
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
        '#default_value' => unserialize($result['dmn_color'])['lname'],
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

function dmn_dmn_edit_page_validate($form, &$form_state)
{

}

/**
 * "Submit logic" for updating data in database
 *
 * @param $form
 * @param $form_state
 */
function dmn_edit_page_submit($form, &$form_state)
{
    $update = dmn_edit_page_db_update($form_state);
    if (isset($update)) {
        drupal_set_message(t('values updated'));
        cache_clear_all('*', 'dmn_cache', TRUE);
        drupal_goto('admin/config/content/dmn/editfields');
    } else {
        drupal_set_message(t('something went wrong'), 'error');
    }
}

function dmn_edit_page_db_update($form_state)
{
    $dmn_color = array();
    $dmn_color['fname'] = $form_state['values']['dmn_fname_color'];
    $dmn_color['lname'] = $form_state['values']['dmn_lname_color'];
    $dmn_color = serialize($dmn_color);

    if (db_update('dmn_table')
        ->fields(array(
            'dmn_fname' => $form_state['values']['dmn_fname'],
            'dmn_lname' => $form_state['values']['dmn_lname'],
            'dmn_info' => $form_state['values']['dmn_info'],
            'dmn_color' => $dmn_color,
        ))
        ->condition('dmnid', $form_state['values']['dmn_id'])
        ->execute()
    )
    {
      return TRUE;
    }
    else
    {
      return FALSE;
    }
}

function dmn_delete($dmnid)
{
    $delete = db_delete('dmn_table')
        ->condition('dmnid', $dmnid)
        ->execute();
    if (isset($delete)) {
        drupal_set_message(t('deleted'));
        cache_clear_all('*', 'dmn_cache', TRUE);
    } else {
        drupal_set_message(t('something went wrong!'), 'error');
    }
    drupal_goto('admin/config/content/dmn/editfields');
}