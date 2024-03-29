<?php

/**
 * @file
 * Implements actions for managing books (book.module).
 */

function views_bulk_operations_book_vboaction_info() {
  $actions = array();
  if (module_exists('book')) {
    $actions['views_bulk_operations_move_to_book_vboaction'] = array(
      'type' => 'node',
      'label' => t('Move to book'),
      'configurable' => TRUE,
      'behavior' => array('changes_property'),
      'vbo' => TRUE,
      //'triggers' => array('any'),
    );
    $actions['views_bulk_operations_remove_from_book_vboaction'] = array(
      'type' => 'node',
      'label' => t('Remove from book'),
      'configurable' => FALSE,
      'vbo' => TRUE,
      //'triggers' => array('any'),
    );
  }

  return $actions;
}

function views_bulk_operations_move_to_book_vboaction_form($context) {
  $form = array();
  if (!isset($context['book'])) {
    $context['book'] = '';
  }
  $options = array();
  $books = book_get_books();
  foreach ($books as $value) {
    $options[$value['nid']] = $value['title'];
  }

  if (empty($options)) {
    backdrop_set_message(t('You have no books.'), 'error');
    return array();
  }

  $form['book'] = array(
    '#type' => 'select',
    '#title' => t('Choose a parent book'),
    '#options' => $options,
    '#description' => t('Select the parent book page you wish to move the book page into'),
  );
  return $form;
}

function views_bulk_operations_move_to_book_vboaction_submit($form, $form_state) {
  return array('book' => $form_state['values']['book']);
}

function views_bulk_operations_move_to_book_vboaction($node, $context = array()) {
  if (isset($context['book'])) {
    $book_node = node_load($context['book']);
    $mlid = db_select('menu_links', 'ml')
      ->condition('ml.link_path', 'node/' . $node->nid)
      ->fields('ml', array('mlid'))
      ->execute()
      ->fetchField();
    $node->book['mlid'] = $mlid;
    $node->book['bid'] = $book_node->nid;
    $node->book['plid'] = $book_node->book['mlid'];
    $node->book['module'] = 'book';
  }
}

/**
 * Adds the action 'Remove node from a parent book'.
 */
function views_bulk_operations_remove_from_book_vboaction($node) {
  if (_book_node_is_removable($node)) {
    menu_link_delete($node->book['mlid']);
    db_delete('book')
      ->condition('nid', $node->nid)
      ->execute();
    backdrop_set_message(t('The post has been removed from the book.'));
  }
  // Remove book to avoid book_node_update() from building the links again.
  unset($node->book);
}
