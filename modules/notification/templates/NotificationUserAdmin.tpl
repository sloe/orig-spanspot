{*
 * $Revision: 847 $
 * If you want to customize this file, do not edit it directly since future upgrades
 * may overwrite it.  Instead, copy it into a new directory called "local" and edit that
 * version.  Gallery will look for that file first and use it if it exists.
 *}

<div class="gbBlock gcBackground1">
    <h2> {g->text text="User Notification Administration"} </h2>
</div>

{if isset($status.saved)}
<div class="gbBlock">
    <h2 class="giSuccess"> {g->text text="Settings saved successfully"} </h2>
</div>
{/if}

{* Methods *}
<div class="gbBlock">
    <table class="gbDataTable">
	<tr>
	    <th colspan="4"> {g->text text="Methods"} </th>
	</tr>
{foreach from=$NotificationMethods item=method name=methods}
    {if $smarty.foreach.methods.iteration % 30 == 1}
	<tr>
	    <th> {g->text text="Name"} </th>
	    <th colspan="3"> {g->text text="Description"} </th>
	</tr>
    {/if}
	<tr>
	    <td>{$method.methodName}</td>
	    <td colspan="3">{$method.methodDescription}</td>
	</tr>
{foreachelse}
	<tr>
	    <td colspan="4"> {g->text text="There are no notification methods available"} </td>
	</tr>
{/foreach}
	<tr>
	    <td colspan="4"> &nbsp; </td>
	</tr>
{* Events *}
	<tr>
	    <th colspan="4"> {g->text text="Events"} </th>
	</tr>
{foreach from=$NotificationSubscriptions key=eventClass item=subscriptions name=subscriptions}
    {assign var="subscriptionCount" value=$subscriptions|@count}
    {if $smarty.foreach.subscriptions.iteration % 30 == 1}
	<tr>
	    <th> {g->text text="Name"} </th>
	    <th> {g->text text="Description"} </th>
	    <th> {g->text text="Method"} </th>
	    <th> {g->text text="Unsubscribe"} </th>
	</tr>
    {/if}
	<tr>
	    <th rowspan="{$subscriptionCount+1}">{$NotificationEvents.$eventClass.eventName}</th>
	    <td colspan="3">{$NotificationEvents.$eventClass.eventDescription}</td>
	</tr>
    {foreach from=$subscriptions item=subscription name=subscription}
	{assign var="subscriptionItem" value="`$subscription.filter`"}
	<tr>
	    <td>
	{if !empty($NotificationItems.$subscriptionItem.url)}
		<a href="{$NotificationItems.$subscriptionItem.url}">{$NotificationItems.$subscriptionItem.title}</a>
	{else}
		{$NotificationItems.$subscriptionItem.title}
	{/if}
	    </td>
	    <td>
	{if $NotificationMethodOptions|@count == 1}
		<input type="hidden" 
		    name="{g->formVar var="form[events][`$eventClass`][methodClass][`$subscriptionItem`]"}" 
		    value="{$NotificationMethodOptions|@key}" />
		{$NotificationMethodOptions|@current}
	{else}
		<select 
		    name="{g->formVar var="form[events][`$eventClass`][methodClass][`$subscriptionItem`]"}" 
		    {html_options options=$NotificationMethodOptions 
			selected=$subscription.methodClass}
		</select>
	{/if}
	    </td>
	    <td><input type="checkbox" 
		name="{g->formVar var="form[events][`$eventClass`][unsubscribe][`$subscriptionItem`]"}" /></td>
	</tr>
    {/foreach}
{foreachelse}
	<tr>
	    <td colspan="4"> {g->text text="You are not subscribed to any available events"} </td>
	</tr>
{/foreach}
    </table>
</div>

<div class="gbBlock gcBackground1">
    <input type="submit" class="inputTypeSubmit"
	name="{g->formVar var="form[action][save]"}" value="{g->text text="Save"}" />
    <input type="reset" class="inputTypeSubmit" value="{g->text text="Reset"}"/>
</div>
