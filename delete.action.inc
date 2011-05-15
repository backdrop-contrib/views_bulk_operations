<?php

function views_bulk_operations_delete_action_info() {
  return array(
    'views_bulk_operations_delete_node_action' => array(
      'type' => 'node',
      'label' => t('Delete node'),
      'configurable' => FALSE,
      'access operation' => VBO_ACCESS_OP_DELETE,
    ),
    'views_bulk_operations_delete_user_action' => array(
      'type' => 'user',
      'label' => t('Delete user'),
      'configurable' => FALSE,
      'access operation' => VBO_ACCESS_OP_DELETE,
    ),
    'views_bulk_operations_delete_comment_action' => array(
      'type' => 'comment',
      'label' => t('Delete comment'),
      'configurable' => FALSE,
      'access operation' => VBO_ACCESS_OP_DELETE,
    ),
    'views_bulk_operations_delete_term_action' => array(
      'type' => 'term',
      'label' => t('Delete term'),
      'configurable' => FALSE,
      'access operation' => VBO_ACCESS_OP_DELETE,
    ),
  );
}

function views_bulk_operations_delete_node_action($node) {
  node_delete($node->nid);
}

function views_bulk_operations_delete_user_action($user) {
  user_delete(null, $user->uid);
}

function views_bulk_operations_delete_comment_action($comment) {
  require_once(drupal_get_path('module', 'comment') .'/comment.admin.inc');
  _comment_delete_thread($comment);
  _comment_update_node_statistics($comment->nid);
}

function views_bulk_operations_delete_term_action($term) {
  taxonomy_del_term($term->tid);
}
