<?php
// $Id$

/**
 * @file
 * Helper functions for the views_node_selector form element type. 
 */

/**
 * Theme function for the views_node_selector form element
 *
 * Display the nodes of a view as a form in a table with radios or checkboxes.
 */
function theme_views_node_selector($element) {
  $fields = _views_get_fields();
  $view = $element['#view'];
  $multiple = $element['#multiple'];
  $output = '';

  // Make sure we have the table header.
  if (!isset($view->table_header)) {
    $view->table_header = _views_construct_header($view, $fields);
  }

  // Add CSS classes for the whole form.
  $class = 'views-form '. ($element['#multiple'] ? 'views-form-multiple' : 'views-form-single');
  if (isset($element['#attributes']['class'])) {
    $class .= ' '. $element['#attributes']['class'];
  }

  $output .=  '<div class = "'. $class .'">';
  if (!empty($element['#view_nodes']) && is_array($element['#view_nodes'])) {
    $rows = array();
    foreach ($element['#view_nodes'] as $node) {
      $arow = array();
      $arow[] = array('data' => ($multiple ? theme('checkbox', $element[$node->nid]) : theme('radio', $element[$node->nid])));
      
      foreach ($view->field as $field) {
        if ($fields[$field['id']]['visible'] !== FALSE) {
          $cell['data'] = views_theme_field('views_handle_field', $field['queryname'],
            $fields, $field, $node, $view);
          $cell['class'] = "view-field ". views_css_safe('view-field-'. $field['queryname']);
          $arow[] = $cell;
        }
      }
      $rows[] = $arow;
    }
  }
  else  {
    $rows[] = array(array('data' => t('No posts available.'), 'colspan' => (count($view->table_header) + 1)));
  }
  // Add a column to the header to accomodate our selection column, and include
  // the 'select all' checkbox if we are using checkboxes.
  $extra = $multiple ? theme('table_select_header_cell') : '&nbsp;';

  array_unshift($view->table_header, $extra);

  $output .= theme('views_form_table', $view->table_header, $rows);
  $output .= '</div>';

  return theme('form_element', $element, $output);
}


/**
 * Default views_node_selector form table theme function.
 */
function theme_views_form_table($header, $rows) {
  return theme('table', $header, $rows);
}

/**
 * True views_node_selector form element expansion function. 
 *
 * @see views_elements() in views.module
 * @see expand_views_node_selector()  in views.module
 * @see form_builder() in Drupal core includes/form.inc
 */
function _expand_views_node_selector($element){
  $options = array();

  foreach($element['#view_nodes'] as $node) {
    $options[$node->nid] = '';
  }

  $element['#options'] = $options;
  if ($element['#multiple']) {
    $element = expand_checkboxes($element);
  }
  else {
    // Handle the case of an array of default values for radios.
    if (is_array($element['#default_value'])) {
      $element['#default_value'] = current($element['#default_value']);
    }
    $element = expand_radios($element);
  }

  // Check for disabled items
  foreach($options as $nid => $v) {
    if (in_array($nid, $element['#disabled_nodes'])) {
      $element[$nid]['#disabled'] = TRUE;
    }
    // Disable the theme function for the radio/checkbox elements so that
    // the output can be built in theme_views_node_selector.
    $element[$nid]['#theme'] = '';
    // Add CSS class.
    $element[$nid]['#attributes']['class'] = 'view-field';
  }

  return $element;
}

/**
 * Pre-process default values when using checkboxes.
 *
 * @see form_builder() in Drupal core includes/form.inc
 */
function views_node_selector_value(&$form) {
  if ($form['#multiple']) {
    checkboxes_value($form);
  }
}

